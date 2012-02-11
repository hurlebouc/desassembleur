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
        printf("0x%lx\t%s\n", prog->VirtualAddr, prog->CompleteInstr);
        
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        Graphe* gIni = &pi[iniAdress - debut];
        gIni->VirtualAddrLue = iniAdress;
        
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
                    
                    if (brancheType == JmpType && cibleAdress - debut < taille) {
                        Graphe* gTarget = &pi[cibleAdress - debut];
                        gTarget->VirtualAddrPointee = cibleAdress;
                        gIni->interet = 1;
                        gTarget->interet = 1;
                        gIni->typeLiaison = JUMP_INCOND;
                        gIni->listeFils = newLinkedList(); // on sait qu on ne risque pas d ecraser d autre fils
                                                           // un jump unconditionnel n a qu un fils
                        addFirstLL(gIni->listeFils, gTarget);
                        //addFirstLL(gIni->listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                        if (gTarget->listePeres == NULL) {
                            gTarget->listePeres = newLinkedList();
                        }
                        addFirstLL(gTarget->listePeres, gIni);
                        //addFirstLL(gTarget->listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        
                    } else if(brancheType!= CallType && brancheType != RetType){
                        if (cibleAdress < taille + debut) {
                            Graphe* gTarget = &pi[cibleAdress - debut];
                            gTarget->VirtualAddrPointee = cibleAdress;
                            gIni->interet = 1;
                            gTarget->interet = 1;
                            gIni->typeLiaison = JUMP_COND;
                            gIni->listeFils = newLinkedList(); // on ne risque pas d ecraser des fils
                            addFirstLL(gIni->listeFils, gTarget); // on ajoute la cible du jump
                            if (gTarget->listePeres==NULL) {
                                gTarget->listePeres = newLinkedList();
                            }
                            addFirstLL(gTarget->listePeres, gIni); // on ajoute l'adresse local aux peres de la cible
                        }
                        if (iniAdress + len < taille + debut) {
                            Graphe* gSuivant = &pi[iniAdress - debut + len];
                            gSuivant->VirtualAddrPointee = iniAdress + len;
                            gIni->interet = 1;
                            gIni->typeLiaison = JUMP_COND;
                            gSuivant->interet = 1;
                            if (gIni->listeFils == NULL) {
                                gIni->listeFils = newLinkedList(); // peut etre a t on deja un fils
                            }
                            addFirstLL(gIni->listeFils, gSuivant); // on ajoute l'instruction suivante
                            if (gSuivant->listePeres == NULL) {
                                gSuivant->listePeres = newLinkedList();
                            }
                            addFirstLL(gSuivant->listePeres, gIni); // on ajoute l'adresse local aux peres de l'instruction suivante
                        }
                    }
                }
                
                if (strcmp(prog->Instruction.Mnemonic, "hlt ") == 0) {
                    gIni->interet = 1;
                    gIni->typeLiaison = FIN;
                }
                
                //printf("%d\n",gIni->interet);
                prog->VirtualAddr += len;
                prog->EIP += len;
                prog->SecurityBlock = (int) (finReel - prog->EIP);
                if (prog->EIP >= finReel) {
                    printf("fin de la lecture\n");
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
        printf("0x%lx\t%s\n", prog->VirtualAddr, prog->CompleteInstr);
        if (strcmp(prog->Instruction.Mnemonic, "hlt ")==0) {
            return;
        }
        
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        long ecart = cibleAdress - iniAdress;
        Graphe* i = &pi[iniAdress - debut];
        i->VirtualAddrLue = iniAdress;
        
        if (i->lu) {
            printf("deja lu\n");
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
                    t->VirtualAddrPointee = cibleAdress;
                    i->interet = 1;
                    i->lu = 1;
                    i->typeLiaison = JUMP_INCOND;
                    t->interet = 1;
                    i->listeFils = newLinkedList();
                    addFirstLL(i->listeFils, t);
                    if (t->listePeres == NULL) {
                        t->listePeres = newLinkedList();
                    }
                    addFirstLL(t->listePeres, i);
                    prog->EIP += ecart;
                    prog->VirtualAddr += ecart;
                    prog->SecurityBlock = (int) (fin - prog->VirtualAddr); // = prog->SecurityBlock - ecart;
                    
                } else if (brancheType == RetType) {
                    unsigned long addr = (long) removeFirstLL(pileAppel); // on depile
                    long decalage = addr - prog->VirtualAddr;
                    prog->VirtualAddr = addr; // += decalage;
                    prog->EIP += decalage;
                    prog->SecurityBlock = (int) (fin - prog->VirtualAddr);
                    Graphe* t = &pi[addr - debut];
                    t->VirtualAddrPointee = addr;
                    t->interet = 1;
                    i->interet = 1;
                    i->lu = 1;
                    i->typeLiaison = JUMP_COND;
                    if (i->listeFils == NULL) {
                        i->listeFils = newLinkedList();
                    }
                    addFirstLL(i->listeFils, t);
                    if (t->listePeres == NULL) {
                        t->listePeres = newLinkedList();
                    }
                    addFirstLL(t->listePeres, i);
                } else {                                // on ne fait qu avancer
                    if (iniAdress + len >= taille + debut) { 
                        printf("depassement anormal du bloc\n");
                        exit(EXIT_FAILURE);
                    }
                    i->lu = 1;
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
 * Cette fonction ne fait pas usage de la pile d'appel. Elle desassemble linéairement 
 * en notant les liens. Cette politique vise à ne pas mélanger les appels au code de
 * la fonction appelante. Chaque fonction aura donc un "tread" qui lui sera dédié
 */


void reperageGlobal(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long finReel = prog->SecurityBlock + prog->EIP;
    unsigned long debut = prog->VirtualAddr;
    unsigned long taille = prog->SecurityBlock;
    unsigned long fin = debut + taille;
    
    while (!stop) {        
        /*-------------- Desassemblage ------------*/
        int len = Disasm(prog);
        printf("0x%lx\t%s\n", prog->VirtualAddr, prog->CompleteInstr);
        
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        //long ecart = cibleAdress - iniAdress;
        Graphe* i = &pi[iniAdress - debut];
        i->VirtualAddrLue = iniAdress;
        
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
                    
                    if (brancheType == JmpType && cibleAdress - debut < taille) { // pour les jmp
                        Graphe* t = &pi[cibleAdress - debut];
                        t->VirtualAddrPointee = cibleAdress;
                        i->interet = 1;
                        t->interet = 1;
                        i->typeLiaison = JUMP_INCOND;
                        i->listeFils = newLinkedList(); // on sait qu on ne risque pas d ecraser d autre fils
                                                           // un jump unconditionnel n a qu un fils
                        addFirstLL(i->listeFils, t);
                        //addFirstLL(gIni->listeFils, (void*) cibleAdress); // on ajoute la cible du jump
                        if (t->listePeres == NULL) {
                            t->listePeres = newLinkedList();
                        }
                        addFirstLL(t->listePeres, i);
                        //addFirstLL(gTarget->listePeres, (void*) iniAdress); // on ajoute l'adresse local aux peres de la cible
                        
                    } else if (brancheType == CallType) { // pour les call
                        i->typeLiaison = CALL;
                        if (cibleAdress < taille + debut) {
                            //addFirstLL(pileAppel, (void*) iniAdress + len); // on empile
                            Graphe* t = &pi[cibleAdress - debut];
                            t->VirtualAddrPointee = cibleAdress;
                            i->interet = 1;
                            //i->lu = 1;
                            t->interet = 1;
                            i->listeFils = newLinkedList();
                            addFirstLL(i->listeFils, t);
                            if (t->listePeres == NULL) {
                                t->listePeres = newLinkedList();
                            }
                            addFirstLL(t->listePeres, i);
                        }
                        if (iniAdress + len < taille + debut) {
                            Graphe* t = &pi[ iniAdress + len - debut];
                            t->VirtualAddrPointee = iniAdress + len;
                            i->interet = 1;
                            //i->lu = 1;
                            t->interet = 1;
                            if (i->listeFils == NULL) {
                                i->listeFils = newLinkedList();
                            }
                            addFirstLL(i->listeFils, t);
                            if (t->listePeres == NULL) {
                                t->listePeres = newLinkedList();
                            }
                            addFirstLL(t->listePeres, i);
                        }
                        
                        
                    } else if(brancheType == RetType){
                        i->typeLiaison = RET;
                        i->interet = 1;
                    } else { // pour tout le reste (jne, jla, ...)
                        if (cibleAdress < taille + debut) {
                            Graphe* t = &pi[cibleAdress - debut];
                            t->VirtualAddrPointee = cibleAdress;
                            i->interet = 1;
                            t->interet = 1;
                            i->typeLiaison = JUMP_COND;
                            i->listeFils = newLinkedList(); // on ne risque pas d ecraser des fils
                            addFirstLL(i->listeFils, t); // on ajoute la cible du jump
                            if (t->listePeres==NULL) {
                                t->listePeres = newLinkedList();
                            }
                            addFirstLL(t->listePeres, i); // on ajoute l'adresse local aux peres de la cible
                        }
                        if (iniAdress + len < taille + debut) {
                            Graphe* s = &pi[iniAdress - debut + len];
                            s->VirtualAddrPointee = iniAdress + len;
                            i->interet = 1;
                            i->typeLiaison = JUMP_COND;
                            s->interet = 1;
                            if (i->listeFils == NULL) {
                                i->listeFils = newLinkedList(); // peut etre a t on deja un fils
                            }
                            addFirstLL(i->listeFils, s); // on ajoute l'instruction suivante
                            if (s->listePeres == NULL) {
                                s->listePeres = newLinkedList();
                            }
                            addFirstLL(s->listePeres, i); // on ajoute l'adresse local aux peres de l'instruction suivante
                        }
                    }
                }
                
                if (strcmp(prog->Instruction.Mnemonic, "hlt ") == 0) {
                    i->interet = 1;
                    i->typeLiaison = FIN;
                }
                
                //printf("%d\n",gIni->interet);
                prog->VirtualAddr += len;
                prog->EIP += len;
                prog->SecurityBlock = (int) (finReel - prog->EIP);
                if (prog->EIP >= finReel) {
                    printf("fin de la lecture\n");
                    stop = 1;
                }
                break;
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
 * TODO : on dirait que dans le cas où g n'est pas le départ d'une flèche
 * alors la tete se retrouve plus loin de prog->VirtualAdress
 *
 * Si on decide que on ne fait pas une fleche depuis les ret, il suffit d ajouter "ret " au "hlt "
 */

void assembleGraphe_aux(DISASM* prog, Graphe* g){
    Disasm(prog);
    printf("\ng : %lx : %lx (%s)\n",g->VirtualAddrLue, prog->VirtualAddr, prog->Instruction.Mnemonic);
    //printf("passage\n");
    if (g->assemble) {
        printf("deja assemble\n");
        return;
    }
    g->assemble = 1;
    
    /*  On verifie que l etat passe est un halt*/
    if (g->typeLiaison == FIN) {
        printf("passage non obligatoire (probleme) (hlt)\n");
        return;
    }
    
    /*  On verifie que l etat passe est un ret*/
    if (g->typeLiaison == RET) {
        printf("passage non obligatoire (probleme) (ret)\n");
        return;
    }
    
    if (g->typeLiaison == JUMP_INCOND) {
        unsigned long VirtualAddrIni = g->VirtualAddrLue;
        printf("un jmp\n");
        //printf("g : %lx\n", g->VirtualAddrLue);
        Graphe* etatCible = g->listeFils->valeur; //on sait listeFils ne contient qu un element
                                                  //etatCible->assemble = 1;
        unsigned long addrCible = etatCible->VirtualAddrLue;
        //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                     // partie d un meme tableau.
        long ecart = addrCible - VirtualAddrIni;
        //printf("ecart : 0x%lx\n", ecart);
        if (ecart>=prog->SecurityBlock) {
            printf("un jump essai de joindre un element en dehors du block\n");
            printf("commande : %lx \t ecart : Ox%lx\n",prog->VirtualAddr, ecart);
            exit(EXIT_FAILURE);
        }
        prog->EIP += ecart;
        prog->VirtualAddr += ecart;
        prog->SecurityBlock = prog->SecurityBlock - (int) ecart;
        assembleGraphe_aux(prog, etatCible);
        return;
    }
    if (g->typeLiaison == JUMP_COND) {
        printf("un jne\n");
        unsigned long EIPini = prog->EIP;
        unsigned long VirtualAddrIni = prog->VirtualAddr;
        unsigned long secuIni = prog->SecurityBlock;
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            unsigned long addrCible = etatCible->VirtualAddrLue;
            //etatCible->assemble=1;
            //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                         // partie d un meme tableau.
            long ecart = addrCible - VirtualAddrIni;
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
    if (g->typeLiaison == CALL) {
        printf("un call\n");
        if (g->listeFils == NULL) {
            printf("erreur de structure : un call n'a pas de fils");
            exit(EXIT_FAILURE);
        }
        unsigned long EIPini = prog->EIP;
        unsigned long VirtualAddrIni = prog->VirtualAddr;
        unsigned long secuIni = prog->SecurityBlock;
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            unsigned long addrCible = etatCible->VirtualAddrLue;
            //etatCible->assemble=1;
            //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
            // partie d un meme tableau.
            long ecart = addrCible - VirtualAddrIni;
            if (ecart>=prog->SecurityBlock) {
                printf("un call essai de joindre un element en dehors du block\n");
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
    
    printf("un non depart d un fleche\n");
    
    Graphe* pi = g; // pi est le tableau des points d internet
                    // dont le premier element est g

    unsigned long debut = prog->VirtualAddr;
    //printf("debut : 0x%lx\n", debut);
    unsigned long fin = prog->VirtualAddr + prog->SecurityBlock;
    
    int len = Disasm2(prog);
    
    
    
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
        printf("depassement du block (anomral)\n");
        exit(EXIT_FAILURE);
    }
    
    Graphe* tete = &(pi[prog->VirtualAddr-debut]);
    //printf("g : %lx, VirtualAddrLue : %lx, VirtualAddr : %lx\n", g->VirtualAddrLue,tete->VirtualAddrLue, prog->VirtualAddr);
    LinkedList* filsUnique = newLinkedList();
    addFirstLL(filsUnique, (void*) tete);
    g->listeFils = filsUnique; // on sait que g n est pas de depart d une fleche
    
    len = Disasm(prog);
    //mnemonic = prog->Instruction.Mnemonic;
    
    while (!tete->interet) {
        
        tete->assemble=1; // normlament on pourrait se contanter des la dernière tete
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
        }
        tete = &(pi[prog->VirtualAddr-debut]);
        //tete->assemble=1; 
        filsUnique->valeur = tete;
        len = Disasm(prog);
        //mnemonic = prog->Instruction.Mnemonic;
    }
    assembleGraphe_aux(prog, tete);
    return;
    
    //if (strcmp(mnemonic, "hlt ") == 0) {
    //    return;
    //} else {
    //    assembleGraphe_aux(prog, tete);
    //    return;
    //}
}

/**
 * Cette fonction assemble les elements du tableau
 * pour en faire un seul graphe
 */

Graphe* assembleGraphe(DISASM* prog, Graphe pi[]){
    Graphe* g = pi; // la premiere instruction est forcement non vide
    assembleGraphe_aux(prog, g);
    g->assemble=1;
    return g;
}

Graphe* ControleFlow(DISASM* prog){
    unsigned long taille = prog->SecurityBlock;
    unsigned long debut = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    Graphe* pi = calloc(sizeof(Graphe),prog->SecurityBlock);
    reperageJump(prog, pi);
    printf("\n\n");
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    reperageAppels(prog, pi);
    afficherPI(pi, taille);
    printf("\n\n");
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    printf("Debut de l assemblage du graphe\n");
    Graphe* g = assembleGraphe(prog, pi);
    printf("Fin de l assemblage du graphe\n");
    return g;
}

Graphe* ControleFlow2(DISASM* prog){
    unsigned long taille = prog->SecurityBlock;
    unsigned long debut = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    Graphe* pi = calloc(sizeof(Graphe),prog->SecurityBlock);
    reperageGlobal(prog, pi);
    printf("\n\n");
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    printf("Debut de l assemblage du graphe\n");
    Graphe* g = assembleGraphe(prog, pi);
    printf("Fin de l assemblage du graphe\n");
    return g;
}

void afficheCF_aux(Graphe* g){
    if (g->affiche) {
        printf("\"%lx\";\n", g->VirtualAddrLue);
        return;
    }
    if (g->listeFils == NULL) {
        printf("\"%lx\";\n", g->VirtualAddrLue);
        g->affiche = 1;
        return;
    }
    g->affiche = 1;
    LinkedList* tete = g->listeFils;
    int totFils = (int) sizeLL(g->listeFils);
    for (int i = 0; i<totFils; i++) { // on visite tous les fils.
        Graphe* etatCible = tete->valeur;
        printf("\"%lx\"->", g->VirtualAddrLue);
        afficheCF_aux(etatCible);
        tete = tete->suiv;
    }
}

void afficheCF(Graphe* g){
    printf("digraph mon_graphe {\n");
    afficheCF_aux(g);
    printf("}\n");
}

void afficheListeFils(Graphe* g){
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<g->listeFils->longueur; i++) {
        Graphe* fils = tete->valeur;
        printf(", %lx", fils->VirtualAddrLue);
        tete = tete->suiv;
    }
    
}

void afficherPI(Graphe* pi, unsigned long taille){
    for (int i = 0; i<taille; i++) {
        if (pi[i].listeFils == NULL) {
            printf("0x%lx\tinteret : %d\n",pi[i].VirtualAddrLue, pi[i].interet);
        } else{
            printf("0x%lx\tinteret : %d \tfils : ",pi[i].VirtualAddrLue, pi[i].interet);
            afficheListeFils(&pi[i]);printf("\n");
        }
    }
}











