#include "dyndesass.h"

int placeRegistreInListe(char* chaine, LinkedList* listeRegistreAppel) {
    int i = 0;
    int fin = 0;
    int taille = (int) listeRegistreAppel->longueur;
    LinkedList* position = listeRegistreAppel;

    while (!fin && i < taille) {
        char* stringValue = position->valeur;
        if (strcmp(stringValue, chaine) == 0) {
            fin = 1;
        } else {
            i++;
            position = position->suiv;
        }
    }
    if (i == taille) {
        addLastLL(listeRegistreAppel, chaine);
    }
    return i;
}

int Disasm2(DISASM* prog){
    int len = Disasm(prog);
    if (prog->VirtualAddr != 0) {
        prog->VirtualAddr += len;
    }
    if (prog->SecurityBlock != 0) {
        prog->SecurityBlock = prog->SecurityBlock - len;
    }
    prog->EIP += len;
    return len;
}


/**
 * Cette fonction desassemble dynamiquement un binaire.
 * Par convention, le point d entree virtuel de _main sera en 0x100000000
 * Celui reel (EIP) devra être initialise en tenant compte du header
 * Le bloc de securite sera aussi initialise.
 * 
 * Tous les autres champs devront etre initialises a zero
 * 
 * On ce conciderera que les saut inconditionnels (sinon, il faut étudier les registres d etat)
 * 
 * @param prog
 */

void desassemblage_inconditionnel(DISASM* prog) {

    LinkedList* pileAppel = newLinkedList();
    int erreur = 0;
    unsigned long debut = prog->EIP;
    unsigned long finProg = prog->EIP + prog->SecurityBlock;
    int len;

    while (!erreur) {
        prog->SecurityBlock = (unsigned int) (finProg - prog->EIP);
        len = Disasm(prog);
        //printf("SecurityBlock = %d et len = %d \n", prog->SecurityBlock, len);
        if (len == UNKNOWN_OPCODE) {
            erreur = 1;
            printf("Code inconnu\n");
            printf("arrete apres %ld octets\n", prog->EIP-debut);
        } else if (len == OUT_OF_BLOCK) {
            erreur = 1;
            printf("Fin du bloc\n");
            printf("arrete apres %ld octets\n", prog->EIP-debut);
        } else {
            unsigned long adresseIni = prog->VirtualAddr;
            printf("(0x%lx) \t 0x%lx \t %s \t (0x%lx)\n", prog->EIP, adresseIni, prog->CompleteInstr, prog->Instruction.AddrValue);
            unsigned long IP = adresseIni + len;
            switch (prog->Instruction.BranchType) {

                case CallType:
                    addFirstLL(pileAppel, (void *) IP); // on empile
                    prog->VirtualAddr = prog->Instruction.AddrValue;
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                case JmpType:
                    prog->VirtualAddr = prog->Instruction.AddrValue;
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                case RetType:
                    prog->VirtualAddr = (unsigned long) pileAppel->valeur;
                    removeFirstLL(pileAppel); // on depile
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                default:
                    prog->VirtualAddr += len;
                    prog->EIP += len;
            }
            if (prog->EIP >= finProg) {
                printf("fin de la lecture");
                printf("arrete apres %ld octets\n", prog->EIP-debut);
                erreur = 1;
            }
        }
    }
}

/**
 * pi est un tableau de Graphes.
 * TODO : il a quelque chose de pourri au royaume du Danemark
 * (dépasement de block)
 */

void reperageJump(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long debut = prog->EIP;
    unsigned long fin = prog->SecurityBlock + debut;
    unsigned long debutVirtuel = prog->VirtualAddr;
    unsigned long taille = prog->SecurityBlock;
    while (!stop) {
        
        /*-------------- Desassemblage ------------*/
        int len = Disasm(prog);
        
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        switch (len) {
            case UNKNOWN_OPCODE:
                printf("opcode inconnu\n");
                exit(EXIT_FAILURE);
                break;
                
            case OUT_OF_BLOCK:
                printf("depassement de bloc\n");
                stop = 1;
                break;
                
            default:
                if (brancheType!=0) {
                    Graphe gIni = pi[iniAdress - debutVirtuel];
                    Graphe gTarget;
                    Graphe gSuivant;
                    if (cibleAdress - debutVirtuel>= taille) {
                        gTarget = pi[cibleAdress - debutVirtuel];
                    }
                    if (iniAdress - debutVirtuel + len >= taille) {
                        gSuivant = pi[iniAdress - debutVirtuel + len];
                    }
                    gIni.interet = 1;
                    gTarget.interet = 1;
                    
                    if (brancheType == JmpType) {
                        gIni.typeLiaison = JUMP_INCOND;
                        LinkedList* listeFils = newLinkedList();
                        addFirstLL(listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                        gIni.listeFils = listeFils;
                        addFirstLL(gTarget.listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        
                    } else if(brancheType!= CallType && brancheType != RetType){
                        gIni.typeLiaison = JUMP_COND;
                        gSuivant.interet = 1;
                        LinkedList* listeFils = newLinkedList();
                        addFirstLL(listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                        addFirstLL(listeFils, (void*) (iniAdress + len)); // on ajoute l'instruction suivante
                        gIni.listeFils = listeFils;
                        addFirstLL(gTarget.listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        addFirstLL(gSuivant.listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de l'instruction suivante
                    }
                    pi[iniAdress - debutVirtuel] = gIni;
                    if (cibleAdress - debutVirtuel>= taille) {
                        pi[cibleAdress - debutVirtuel] = gTarget;
                    }
                    if (iniAdress - debutVirtuel + len >= taille) {
                        pi[iniAdress - debutVirtuel + len] = gSuivant;
                    }
                }
                
                
                prog->VirtualAddr += len;
                prog->EIP += len;
                prog->SecurityBlock = (int) (fin - prog->EIP);
                if (prog->EIP >= fin) {
                    printf("fin de la lecture");
                    stop = 1;
                }
                break;
        }
    }
}

void reperageAppels(DISASM* prog, Graphe pi[]){
    
}

/**
 * le parametre pi est la positon dans le tableau
 * 
 * au moement ou un fait la reccursion, il faut verifier que on ne passe pas 
 * une adresse en dehors du block.
 * 
 * le graphe passe est l un des fils de l iteration precedente
 * 
 * prog et g designent la meme chose
 * 
 * dans tous les cas, g sera un pi ou le debut du programme
 * 
 * cette fonction est reccursive
 */

void assembleGraphe_aux(DISASM* prog, Graphe* g){
    
    if (g->typeLiaison == JUMP_INCOND) {
        Graphe* etatCible = g->listeFils->valeur; //on sait listeFils ne contient qu un element
        long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                     // partie d un meme tableau.
        if (ecart>=prog->SecurityBlock) {
            printf("un jump essai de joindre un element en dehors du block\n");
            exit(EXIT_FAILURE);
        }
        prog->EIP += ecart;
        prog->VirtualAddr += ecart;
        prog->SecurityBlock = prog->SecurityBlock - (int) ecart;
        assembleGraphe_aux(prog, etatCible);
        return;
    }
    if (g->typeLiaison == JUMP_COND) {
        unsigned long EIPini = prog->EIP;
        unsigned long VirtualAddrIni = prog->VirtualAddr;
        unsigned long secuIni = prog->SecurityBlock;
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) {
            Graphe* etatCible = tete->valeur;
            long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                         // partie d un meme tableau.
            if (ecart>=prog->SecurityBlock) {
                printf("un jump essai de joindre un element en dehors du block\n");
                exit(EXIT_FAILURE);
            }
            prog->EIP = EIPini + ecart;
            prog->VirtualAddr = VirtualAddrIni + ecart;
            prog->SecurityBlock = (int) (secuIni - ecart);
            assembleGraphe_aux(prog, etatCible);
            tete = tete->suiv;
        }
        return;
    }
    
    /* on fait maintenant le cas ou g n est pas le depart d une fleche */
    
    Graphe* pi = g; // pi est le tableau des points d internet
                    // dont le premier element est g

    unsigned long debut = prog->VirtualAddr;
    unsigned long fin = prog->VirtualAddr + prog->SecurityBlock;
    
    int len = Disasm2(prog);
    
    /*  On verifie que l etat passe n est pas un halt  (pas besoin nomalement) */
    char* mnemonic = prog->Instruction.Mnemonic;
    if (strcmp(mnemonic, "hlt ") == 0) {
        return;
    }
    
    // normalement on a pas besoin de verifier si on sort du block
    if (prog->VirtualAddr>=fin) {
        printf("depassement de la lecture (normalement impossible)\n");
        exit(EXIT_FAILURE);
    }if (len == UNKNOWN_OPCODE) {
        printf("opcode inconnu\n");
        exit(EXIT_FAILURE);
    }
    if (len == OUT_OF_BLOCK) {
        printf("depassement du block (anomral)\n");
        exit(EXIT_FAILURE);
    }
    
    Graphe* tete = &(pi[prog->VirtualAddr-debut]);
    LinkedList* filsUnique = newLinkedList();
    addFirstLL(filsUnique, (void*) tete);
    g->listeFils = filsUnique; // on sait que g n est pas de depart d une fleche
    
    len = Disasm(prog);
    mnemonic = prog->Instruction.Mnemonic;
    
    while (!tete->interet && strcmp(mnemonic, "hlt ") != 0) {
        
        prog->EIP += len;
        prog->VirtualAddr +=len;
        prog->SecurityBlock = prog->SecurityBlock - len;
        
        // normalement on a pas besoin de verifier si on sort du block
        if (prog->VirtualAddr>=fin) {
            printf("depassement de la lecture (normalement impossible)\n");
            exit(EXIT_FAILURE);
        }
        if (len == UNKNOWN_OPCODE) {
            printf("opcode inconnu\n");
            exit(EXIT_FAILURE);
        }
        if (len == OUT_OF_BLOCK) {
            printf("depassement du block (pas normal)\n");
            exit(EXIT_FAILURE);
        } else {
            tete = &(pi[prog->VirtualAddr-debut]);
            filsUnique->valeur = tete;
            len = Disasm(prog);
            mnemonic = prog->Instruction.Mnemonic;
        }
    }
    if (strcmp(mnemonic, "hlt ") == 0) {
        return;
    } else {
        assembleGraphe_aux(prog, tete);
        return;
    }
}

/**
 * Cette fonction assemble les elements du tableau
 * pour en faire un seul graphe
 */

Graphe* assembleGraphe(DISASM* prog, Graphe pi[]){
    Graphe* g = pi; // la premiere instruction est forcement non vide
    assembleGraphe_aux(prog, g);
    return g;
}

Graphe* ControleFlow(DISASM* prog){
    unsigned long taille = prog->SecurityBlock;
    unsigned long debut = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    Graphe* pi = calloc(sizeof(Graphe),prog->SecurityBlock);
    reperageJump(prog, pi);
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    reperageAppels(prog, pi);
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    return assembleGraphe(prog, pi);
}

