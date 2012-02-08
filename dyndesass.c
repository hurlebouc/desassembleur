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
                printf("fin de la lecture\n");
                printf("arrete apres %ld octets\n", prog->EIP-debut);
                erreur = 1;
            }
        }
    }
}

/**
 * pi est un tableau de Graphes.
 * TODO : il a quelque chose de pourri au royaume du Danemark
 * (dépasement de block) (OK)
 */

void reperageJump(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long finReel = prog->SecurityBlock + prog->EIP;
    unsigned long debut = prog->VirtualAddr;
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
                    Graphe* gIni = &pi[iniAdress - debut];
                    Graphe* gTarget = &pi[cibleAdress - debut];
                    Graphe* gSuivant = &pi[iniAdress - debut + len];
                    
                    
                    if (brancheType == JmpType && cibleAdress - debut < taille) {
                        gIni->interet = 1;
                        gTarget->interet = 1;
                        gIni->typeLiaison = JUMP_INCOND;
                        gIni->listeFils = newLinkedList(); // on sait qu on ne risque pas d ecraser d autre fils
                                                           // un jump unconditionnel n a qu un fils
                        addFirstLL(gIni->listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                        if (gTarget->listePeres == NULL) {
                            gTarget->listePeres = newLinkedList();
                        }
                        addFirstLL(gTarget->listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        
                    } else if(brancheType!= CallType && brancheType != RetType){
                        if (cibleAdress < taille + debut) {
                            gIni->interet = 1;
                            gTarget->interet = 1;
                            gIni->typeLiaison = JUMP_COND;
                            gIni->listeFils = newLinkedList(); // on ne risque pas d ecraser des fils
                            addFirstLL(gIni->listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                            if (gTarget->listePeres==NULL) {
                                gTarget->listePeres = newLinkedList();
                            }
                            addFirstLL(gTarget->listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        }
                        if (iniAdress + len < taille + debut) {
                            gIni->interet = 1;
                            gIni->typeLiaison = JUMP_COND;
                            gSuivant->interet = 1;
                            if (gIni->listeFils == NULL) {
                                gIni->listeFils = newLinkedList(); // peut etre a t on deja un fils
                            }
                            addFirstLL(gIni->listeFils, (void*) (iniAdress + len)); // on ajoute l'instruction suivante
                            if (gSuivant->listePeres == NULL) {
                                gSuivant->listePeres = newLinkedList();
                            }
                            addFirstLL(gSuivant->listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de l'instruction suivante
                        }
                    }
                }
                
                
                prog->VirtualAddr += len;
                prog->EIP += len;
                prog->SecurityBlock = (int) (finReel - prog->EIP);
                if (prog->EIP >= finReel) {
                    printf("fin de la lecture");
                    stop = 1;
                }
                break;
        }
    }
}

/**
 * Cette fonction se limitera dans un premier temps aux cas courant (non pathologiques)
 * On conciderera que lors d un appel, seuls deux cas sont possibles
 * Soit on tombe sur du code non lu. Dans ce cas, la fonction appele n a jamais ete lue avant
 * Soit la premier ligne de code a deja ete lue. Dans ce cas, la fonction a deja deja ete lue 
 * (partiellement si la fcontion est reccursive) et on retourne au call appelant.
 * Pour faire un peu plus general, on programmera le retour au dernier call appelant des que 
 * on rencoutrera du code deja lu.
 * On gardera quand meme dans le call appelant la trace de cet appel (pi[]).
 */

void reperageAppels(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long debut = prog->VirtualAddr;
    unsigned long fin = prog->SecurityBlock + debut;
    unsigned long taille = prog->SecurityBlock;
    LinkedList* pileAppel = newLinkedList();
    while (!stop) {
        
        /*-------------- Desassemblage ------------*/
        int len = Disasm(prog);
        
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        long ecart = cibleAdress - iniAdress;
        Graphe* i = &pi[iniAdress - debut];
        if (i->lu) {
            unsigned long addr = (long) removeFirstLL(pileAppel); // on depile
            long decalage = addr - prog->VirtualAddr;
            prog->VirtualAddr = addr; // += decalage;
            prog->EIP += decalage;
            prog->SecurityBlock = (int) (fin - prog->VirtualAddr);
            len = -100;
        }
        switch (len) {
            case -100:
                break;
                
            case UNKNOWN_OPCODE:
                printf("opcode inconnu\n");
                exit(EXIT_FAILURE);
                break;
                
            case OUT_OF_BLOCK:
                printf("depassement de bloc\n");
                stop = 1;
                break;
                
            default:
                if (brancheType == CallType && cibleAdress < taille + debut) {
                    addFirstLL(pileAppel, (void*) iniAdress + len); // on empile
                    Graphe* t = &pi[cibleAdress - debut];
                    i->interet = 1;
                    i->typeLiaison = JUMP_INCOND;
                    t->interet = 1;
                    t->lu = 1;
                    i->listeFils = newLinkedList();
                    addFirstLL(i->listeFils, (void*) cibleAdress);
                    if (t->listePeres == NULL) {
                        t->listePeres = newLinkedList();
                    }
                    addFirstLL(t->listePeres, (void*) iniAdress);
                    prog->EIP += ecart;
                    prog->VirtualAddr += ecart;
                    prog->SecurityBlock = (int) (fin - prog->VirtualAddr); // = prog->SecurityBlock - ecart;
                }
                if (brancheType == RetType) {
                    unsigned long addr = (long) removeFirstLL(pileAppel); // on depile
                    long decalage = addr - prog->VirtualAddr;
                    prog->VirtualAddr = addr; // += decalage;
                    prog->EIP += decalage;
                    prog->SecurityBlock = (int) (fin - prog->VirtualAddr);
                    Graphe* t = &pi[addr - debut];
                    t->interet = 1;
                    t->lu = 1;
                    if (i->listeFils == NULL) {
                        i->listeFils = newLinkedList();
                    }
                    addFirstLL(i->listeFils, (void*) addr);
                    if (t->listePeres == NULL) {
                        t->listePeres = newLinkedList();
                    }
                    addFirstLL(t->listePeres, (void*) iniAdress);
                }
                if (brancheType == 0) {
                    if (iniAdress + len >= taille + debut) {
                        printf("depassement anormal du bloc\n");
                        exit(EXIT_FAILURE);
                    }
                    Graphe* t = &pi[iniAdress + len - debut];
                    t->lu = 1;
                    prog->EIP+= len;
                    prog->VirtualAddr += len;
                    prog->SecurityBlock = (int) (fin - prog->VirtualAddr); // = prog->SecurityBlock - len;
                }
                break;
        }
        if (prog->VirtualAddr >= fin) {
            printf("fin de la lecture");
            stop = 1;
        }
    }
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
 *
 * TODO : ENCORE UN TRUC DE POURRI (ne termine pas) : OK
 *
 * Si on decide que on ne fait pas une fleche depuis les ret, il suffit d ajouter "ret " au "hlt "
 */

void assembleGraphe_aux(DISASM* prog, Graphe* g){
    
    if (g->assemble) {
        return;
    }
    
    if (g->typeLiaison == JUMP_INCOND) {
        Graphe* etatCible = g->listeFils->valeur; //on sait listeFils ne contient qu un element
        etatCible->assemble = 1;
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
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            etatCible->assemble=1;
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
    tete->assemble=1;
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
            tete->assemble=1; // nomrlament on pourrait se contanter des la dernière tete
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
    g->assemble=1;
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

