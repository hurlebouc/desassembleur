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

/*--------------------------------------------------------------------------------------------------*/
/*                                                                                                  */
/*                                         PRE-TRAITEMENT                                           */
/*                                                                                                  */
/*--------------------------------------------------------------------------------------------------*/


void fermeture(DISASM* prog, int* crible){
    
    LinkedList* pileAppel = newLinkedList();
    int stop = 0;
    unsigned long debut = prog->VirtualAddr;
    unsigned long taille = prog->SecurityBlock;
    unsigned long fin = taille + debut;
    
    while (!stop) {
        //prog->SecurityBlock = (unsigned int) (finReel - prog->EIP); // !!!!!!!!!!!!!!!!!        A SUPPRIMER
        
        int len = Disasm(prog);
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!       ATTENTION AU CAS NUL
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        unsigned long IP = iniAdress + len;
        printf("0x%lx \t %s \t (0x%lx)\n", iniAdress, prog->CompleteInstr, cibleAdress);
        
        if (len == UNKNOWN_OPCODE) {
            printf("Code inconnu\n");
            crible[iniAdress-debut] = -1;
            if (pileAppel->longueur == 0) {
                printf("warning : le desassemblage s'est terminé sans rencontrer de point d'arret\n");
                exit(EXIT_SUCCESS);
            }
            unsigned long retourAddr = (unsigned long) removeFirstLL(pileAppel);
            prog->VirtualAddr = retourAddr;
            prog->EIP += retourAddr - (long) iniAdress;
            
        } else if (len == OUT_OF_BLOCK) {
            printf("Fin du bloc\n");
            crible[iniAdress-debut] = -1;
            if (pileAppel->longueur == 0) {
                printf("warning : le desassemblage s'est terminé sans rencontrer de point d'arret\n");
                exit(EXIT_SUCCESS);
            }
            unsigned long retourAddr = (unsigned long) removeFirstLL(pileAppel);
            prog->VirtualAddr = retourAddr;
            prog->EIP += retourAddr - (long) iniAdress;
            
        } else if (crible[iniAdress - debut]){ // si on est deja passé par la
            if (pileAppel->longueur == 0) {
                printf("warning : le desassemblage s'est terminé sans rencontrer de point d'arret\n");
                exit(EXIT_SUCCESS);
            }
            unsigned long retourAddr = (unsigned long) removeFirstLL(pileAppel);
            prog->VirtualAddr = retourAddr;
            prog->EIP += retourAddr - (long) iniAdress;
            
        } else {
            crible[iniAdress-debut] = 1;
            switch (brancheType) {
                    
                case CallType:
                    if (cibleAdress == 0) {
                        printf("call indéfini\n");
                    }
                    if (cibleAdress < fin && cibleAdress != 0) {
                        addFirstLL(pileAppel, (void *) IP); // on empile
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                    } else if(prog->VirtualAddr + len < fin){
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {
                        printf("erreur : un call se trouve en derniere position du bloc\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                    
                case JmpType:
                    if (cibleAdress == 0) { // dans ce cas on dépile car on ne sait rien faire d'autre
                        printf("saut inconditionnel indéfini\n");
                        if (pileAppel->longueur == 0) {
                            printf("warning : le desassemblage s'est terminé sans rencontrer de point d'arret\n");
                            exit(EXIT_SUCCESS);
                        }
                        prog->VirtualAddr = (unsigned long) removeFirstLL(pileAppel);
                        prog->EIP += prog->VirtualAddr - (long) iniAdress;
                    } else if (cibleAdress < fin) {
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                    } else {
                        printf("erreur : un jump essai de sortir du bloc \n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                    
                case RetType:
                    if (pileAppel->longueur == 0) {
                        printf("warning : le desassemblage s'est terminé sans rencontrer de point d'arret\n");
                        exit(EXIT_SUCCESS);
                    }
                    prog->VirtualAddr = (unsigned long) removeFirstLL(pileAppel);
                    prog->EIP += prog->VirtualAddr - (long) iniAdress;
                    break;
                    
                case 0: // cas ou il n y a pas de saut
                    if (strcmp(prog->Instruction.Mnemonic, "hlt ")==0) {
                        printf("fin de la lecture\n\n");
                        stop = 1;
                    }else if (prog->VirtualAddr + len < fin) {
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {
                        printf("erreur : la fin est atteinte sans rencontrer de point d arret");
                        exit(EXIT_FAILURE);
                    }
                    break;
                    
                default: // cas des jumps conditionnels
                    if (cibleAdress == 0) {
                        printf("saut conditionnel indéfini\n");
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    }
                    if (cibleAdress < fin) {
                        addFirstLL(pileAppel, (void *) IP); // on empile
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                    } else {
                        printf("erreur : un jump conditionnel essai de sortir du bloc\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                   
            }
        }
        prog->SecurityBlock = (unsigned int) (fin - prog->VirtualAddr);
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
 *
 * TODO :   Pour améliorer la fonction, il faut parcourir tout le binaire et pas 
 *          seulement les instructions accessible en première lecture. Comme ça
 *          on echappe au piège des junk bytes.
 * TODO :   Améliorer la gestion des saut non définis. Il faut que les jumps inconditionnels 
 *          dans cette situation soient la fin d'un thread.
 */


void reperageGlobal(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long finReel = prog->SecurityBlock + prog->EIP;
    unsigned long debut = prog->VirtualAddr;
    unsigned long taille = prog->SecurityBlock;
    //unsigned long fin = debut + taille;
    
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
                i->interet = -1;
                //exit(EXIT_FAILURE);
                break;
                
            case OUT_OF_BLOCK:
                printf("depassement de bloc pendant la lecture\n");
                i->interet = -1;
                //stop = 1;
                break;
                
            default:
                if (cibleAdress == 0 && brancheType !=0) {
                    printf("cible non définie\n");
                }
                if (brancheType!=0) {
                    
                    if (brancheType == JmpType && cibleAdress - debut < taille  && cibleAdress != 0) { // pour les jmp
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
                        if (cibleAdress < taille + debut && cibleAdress != 0) {
                            //addFirstLL(pileAppel, (void*) iniAdress + len); // on empile
                            Graphe* t = &pi[cibleAdress - debut];
                            t->VirtualAddrPointee = cibleAdress;
                            i->interet = 1;
                            //i->lu = 1;
                            t->interet = 1;
                            t->debutFonction = 1;
                            i->listeFils = newLinkedList();
                            addFirstLL(i->listeFils, t);
                            if (t->listePeres == NULL) {
                                t->listePeres = newLinkedList();
                            }
                            addFirstLL(t->listePeres, i);
                        }
                        if (iniAdress + len < taille + debut) {
                            Graphe* t = &pi[iniAdress + len - debut];
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
                    } else if(brancheType == RetType){ //pour les ret
                        i->typeLiaison = RET;
                        i->interet = 1;
                    } else { // pour tout le reste (jne, jla, ...)
                        if (cibleAdress < taille + debut && cibleAdress != 0) {
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
                break;
        }
        if (strcmp(prog->Instruction.Mnemonic, "hlt ") == 0) {
            i->interet = 1;
            i->typeLiaison = FIN;
        }
        
        //printf("%d\n",gIni->interet);
        prog->VirtualAddr += len;
        prog->EIP += len;
        prog->SecurityBlock = (int) (finReel - prog->EIP);
        prog->Instruction.BranchType = 0;
        prog->Instruction.AddrValue = 0;
        if (prog->EIP >= finReel) {
            printf("fin de la lecture\n");
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
        printf("un hlt\n");
        return;
    }
    
    /*  On verifie que l etat passe est un ret*/
    if (g->typeLiaison == RET) {
        printf("un ret\n");
        return;
    }
    
    if (g->typeLiaison == JUMP_INCOND) {
        unsigned long VirtualAddrIni = g->VirtualAddrLue;
        unsigned long fin = VirtualAddrIni + prog->SecurityBlock;
        printf("un jmp\n");
        //printf("g : %lx\n", g->VirtualAddrLue);
        Graphe* etatCible = g->listeFils->valeur; //on sait listeFils ne contient qu un element
                                                  //etatCible->assemble = 1;
        unsigned long addrCible = etatCible->VirtualAddrLue;
        //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                     // partie d un meme tableau.
        long ecart = addrCible - VirtualAddrIni;
        //printf("ecart : 0x%lx\n", ecart);
        if (addrCible >= fin) {
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
        long secuIni = prog->SecurityBlock;
        unsigned long fin = VirtualAddrIni + secuIni;
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            unsigned long addrCible = etatCible->VirtualAddrLue;
            //etatCible->assemble=1;
            //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
                                                         // partie d un meme tableau.
            long ecart = addrCible - VirtualAddrIni;
            if (addrCible >= fin) {
                printf("un jump essai de joindre un element en dehors du block\n");
                exit(EXIT_FAILURE);
            }
            prog->EIP = EIPini + ecart;
            prog->VirtualAddr = addrCible;
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
        long secuIni = prog->SecurityBlock;
        unsigned long fin = VirtualAddrIni + secuIni;
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            unsigned long addrCible = etatCible->VirtualAddrLue;
            //etatCible->assemble=1;
            //long ecart = (etatCible - g)/sizeof(Graphe); // on n oublie pas que tous les graphes font
            // partie d un meme tableau.
            long ecart = addrCible - VirtualAddrIni;
            if (addrCible >= fin) {
                printf("un call essai de joindre un element en dehors du block\n");
                printf("adresse du call : %lx\n", VirtualAddrIni);
                exit(EXIT_FAILURE);
            }
            prog->EIP = EIPini + ecart;
            prog->VirtualAddr = addrCible;
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
        printf("\nerreur : la ligne 0x%lx est hors du bloc\n",prog->VirtualAddr);
        exit(EXIT_FAILURE);
    }
    if (len == UNKNOWN_OPCODE) {
        printf("\nerreur : opcode inconnu\n");
        exit(EXIT_FAILURE);
    }
    if (len == OUT_OF_BLOCK) {
        printf("\nerreur : depassement du bloc lors de la lecture d un instruction\n");
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
            printf("\nerreur : la ligne 0x%lx est hors du bloc\n",prog->VirtualAddr);
            exit(EXIT_FAILURE);
        }
        if (len == UNKNOWN_OPCODE) {
            printf("\nerreur : opcode inconnu\n");
            exit(EXIT_FAILURE);
        }
        if (len == OUT_OF_BLOCK) {
            printf("\nerreur : depassement du bloc lors de la lecture d un instruction\n");
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
    prog->SecurityBlock = (unsigned int) taille;
    prog->VirtualAddr = virtualAddr;
    reperageAppels(prog, pi);
    afficherPI(pi, taille);
    printf("\n\n");
    
    prog->EIP = debut;
    prog->SecurityBlock = (unsigned int) taille;
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
    prog->SecurityBlock = (unsigned int) taille;
    prog->VirtualAddr = virtualAddr;
    printf("Debut de l assemblage du graphe\n");
    Graphe* g = assembleGraphe(prog, pi);
    printf("Fin de l assemblage du graphe\n");
    return g;
}

/*--------------------------------------------------------------------------------------------------*/
/*                                                                                                  */
/*                                           AFFICHAGE                                              */
/*                                                                                                  */
/*--------------------------------------------------------------------------------------------------*/

void afficheCF_aux(Graphe* g){
    if (g->affiche) {
        printf("\"%lx\";\n", g->VirtualAddrLue);
        return;
    }
    if (g->listeFils == NULL) {
        printf("\"%lx\"", g->VirtualAddrLue);
        g->affiche = 1;
        if (g->typeLiaison == RET) {
            printf("[style=filled fillcolor=grey]");
        }
        printf(";\n");
        return;
    }
    g->affiche = 1;
    LinkedList* tete = g->listeFils;
    int totFils = (int) sizeLL(g->listeFils);
    for (int i = 0; i<totFils; i++) { // on visite tous les fils.
        Graphe* etatCible = tete->valeur;
        printf("\"%lx\"->\"%lx\"", g->VirtualAddrLue, etatCible->VirtualAddrLue);
        if (g->typeLiaison == CALL) {
            if (etatCible->debutFonction) {
                printf(" [color=red];\n");
            } else {
                printf(";\n");
            }
            printf("\"%lx\" [style=filled fillcolor=red]", g->VirtualAddrLue);
        }
        if (g->typeLiaison == JUMP_INCOND) {
            printf(" [color=blue];\n");
            printf("\"%lx\" [style=filled fillcolor=blue]", g->VirtualAddrLue);
        }
        if (g->typeLiaison == JUMP_COND) {
            printf(" [color=green];\n");
            printf("\"%lx\" [style=filled fillcolor=green]", g->VirtualAddrLue);
        }
        printf(";\n");
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

void afficheCrible(int* crible, unsigned long taille, unsigned long pev){
    for (unsigned long i = 0; i<taille; i++) {
        printf("%lx \t %d\n", pev + i, crible[i]);
    }
}











