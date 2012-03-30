#include "dyndesass.h"



static int placeRegistreInListe(char* chaine, LinkedList* listeRegistreAppel) {
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

static int Disasm2(DISASM* prog){
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

/*--------------------------------------------------------------------------------------------------*/
/*                                                                                                  */
/*                                         PRE-TRAITEMENT                                           */
/*                                                                                                  */
/*--------------------------------------------------------------------------------------------------*/

static int depilage(DISASM* prog, LinkedList* pileAppel, Fichier* fichier){
    unsigned long iniAdress = prog->VirtualAddr;
    if (pileAppel->longueur == 0) {
        pushlog(fichier, "\nArret du désassembleur par dépilage d'une pile vide\n\n");
//        fprintf(graveur, "\nArret du désassembleur par dépilage d'une pile vide\n\n");
        return 1;
        //exit(EXIT_SUCCESS);
    }
    unsigned long retourAddr = (unsigned long) removeFirstLL(pileAppel);
    prog->VirtualAddr = retourAddr;
    prog->EIP += retourAddr - (long) iniAdress;
    return 0;
}

void fermeture(desasembleur* desas, Graphe pi[]){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
//    strcat(chemin_log, "/fermeture.log");
    strcat(chemin_log, CHEMIN_LOG_FERMETURE);
    Fichier* fichierlog = newFichier(chemin_log);
    
    char temp[MAX_BUFFER];
//    FILE* graveur;
    
//    printf("%s\n",CHEMIN_LOG);
    DISASM* prog = desas->disasm;
    LinkedList* pileAppel = newLinkedList();
    int stop = 0;
    unsigned long debut = desas->debutVirtuel;
    //unsigned long taille = prog->SecurityBlock;
    unsigned long fin = prog->SecurityBlock + prog->VirtualAddr;
    
    while (!stop) {
        
        int len = Disasm(prog);
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        unsigned long IP = iniAdress + len;
        sprintf(temp,  "0x%lx \t %s \t (0x%lx)\n", iniAdress, prog->CompleteInstr, cibleAdress);
        pushlog(fichierlog, temp);
        sprintf(temp, "%d  %d  %d  %d  %d  %d  %d  %d  %d  %d  %d  \n",
                prog->Instruction.Flags.OF_,
                prog->Instruction.Flags.SF_, 
                prog->Instruction.Flags.ZF_, 
                prog->Instruction.Flags.AF_, 
                prog->Instruction.Flags.PF_, 
                prog->Instruction.Flags.CF_, 
                prog->Instruction.Flags.TF_, 
                prog->Instruction.Flags.IF_, 
                prog->Instruction.Flags.DF_, 
                prog->Instruction.Flags.NT_,
                prog->Instruction.Flags.RF_);
        pushlog(fichierlog, temp);
        Graphe* i = &pi[iniAdress - debut];
        i->VirtualAddrLue = iniAdress;
        i->tailleInstruction = len;
        for (int k = 1; k<len; k++) {
            if (iniAdress + k < fin) {
                pi[iniAdress + k  - debut].recouvert = EST_RECOUVERT;
            }
        }
                
        if (len == UNKNOWN_OPCODE) {
            pushlog(fichierlog, "WARNING : le desassembleur a rencontré un opcode inconnu\n");
            i->interet = OPCODE_INCONNU;
            i->lu = EST_LU;
            stop = depilage(prog, pileAppel, fichierlog);
            
        } else if (len == OUT_OF_BLOCK) {
            pushlog(fichierlog, "WARNING : le désassembleur a rencontré un depassement de bloc lors de la lecture d'une instruction\n");
            i->interet = DEPASSEMENT_BLOC;
            i->lu = EST_LU;
            stop = depilage(prog, pileAppel, fichierlog);
            
        } else if (/*crible[iniAdress - debut] != 0*/ i->lu == EST_LU){ // si on est deja passé par là
            stop = depilage(prog, pileAppel, fichierlog); 
                        
        } else {
            i->lu = EST_LU;
            switch (brancheType) {
                    
                case CallType:
                    i->interet = GO_AND_LEAVE;
                    i->typeLiaison = CALL;
                    if (cibleAdress == 0) {
                        pushlog(fichierlog, "WARNING : call indéfini\n");
                        i->interet = CALL_INDEFINI;
                    }
                    if (cibleAdress < fin && cibleAdress >= debut) {
                        Graphe* t = &pi[cibleAdress - debut];
                        t->VirtualAddrPointee = cibleAdress;
                        t->interet = GO_AND_LEAVE;
                        t->debutFonction = 1;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, t);
                        if (t->listePeres == NULL) {
                            t->listePeres = newLinkedList();
                        }
                        addFirstLL(t->listePeres, i);
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                        if (iniAdress + len < fin) {
                            Graphe* s = &pi[iniAdress + len - debut];
                            s->VirtualAddrPointee = iniAdress + len;
                            s->interet = GO_AND_LEAVE;
                            addFirstLL(i->listeFils, s);
                            if (s->listePeres == NULL) {
                                s->listePeres = newLinkedList();
                            }
                            addFirstLL(s->listePeres, i);
                            addFirstLL(pileAppel, (void *) IP); // on empile
                        } else {
                            pushlog(fichierlog, "WARNING : il est impossible de revenir à ce call car");
                            pushlog(fichierlog,  "il est la dernière instruction du bloc\n");
                            i->interet = CALL_FIN_BLOC;
                        }
                    } else if(iniAdress + len < fin){ // cas où le call appel une fonction
                                                      // hors du bloc ou lorsque le saut est indéfini
                        if (cibleAdress != 0) {
                            pushlog(fichierlog, "WARNING : un call fait appel à une fonction hors du bloc\n");
                            i->interet = CALL_OUT_OF_BLOCK;
                        }
                        Graphe* s = &pi[iniAdress + len - debut];
                        s->VirtualAddrPointee = iniAdress + len;
                        s->interet = GO_AND_LEAVE;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, s);
                        if (s->listePeres == NULL) {
                            s->listePeres = newLinkedList();
                        }
                        addFirstLL(s->listePeres, i);
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {
                        pushlog(fichierlog, "WARNING : un call n'a aucun fils\n");
                        i->interet = CALL_TERMINAL;
                        stop = depilage(prog, pileAppel, fichierlog);
                    }
                    break;
                    
                case JmpType:
                    i->interet = GO_AND_LEAVE;
                    i->typeLiaison = JUMP_INCOND;
                    if (cibleAdress == 0) {
                        pushlog(fichierlog, "WARNING : saut inconditionnel indéfini\n");
                        i->interet = SAUT_INCOND_INDEFINI;
                        stop = depilage(prog, pileAppel, fichierlog);
                    } else if (cibleAdress < fin && cibleAdress >= debut) {
                        Graphe* t = &pi[cibleAdress - debut];
                        t->VirtualAddrPointee = cibleAdress;
                        t->interet = GO_AND_LEAVE;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, t);
                        if (t->listePeres == NULL) {
                            t->listePeres = newLinkedList();
                        }
                        addFirstLL(t->listePeres, i);
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                    } else {
                        pushlog(fichierlog, "WARNING : un jump essai de sortir du bloc\n");
                        i->interet = SAUT_INCOND_OUT_OF_BLOCK;
                        stop = depilage(prog, pileAppel, fichierlog);
                    }
                    break;
                    
                case RetType:
                    i->interet = GO_AND_LEAVE;
                    i->typeLiaison = RET;
                    stop = depilage(prog, pileAppel, fichierlog);
                    break;
                    
                case 0: // cas ou il n y a pas de saut
                    if (strcmp(prog->Instruction.Mnemonic, "hlt ")==0) {
                        i->interet = GO_AND_LEAVE;
                        i->typeLiaison = FIN;
                        stop = depilage(prog, pileAppel, fichierlog);
                    }else if (iniAdress + len < fin) {
                        Graphe* s = &pi[iniAdress + len - debut];
                        s->VirtualAddrPointee = iniAdress + len;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, s);
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {
                        pushlog(fichierlog, "WARNING : la fin du bloc est atteinte sans rencontrer de point d arret");
                        i->interet = FIN_BLOC_SANS_POINT_ARRET;
                        stop = depilage(prog, pileAppel, fichierlog);
                    }
                    break;
                    
                default: // cas des jumps conditionnels
                    i->interet = GO_AND_LEAVE;
                    i->typeLiaison = JUMP_COND;
                    if (cibleAdress == 0) {
                        pushlog(fichierlog, "WARNING : saut conditionnel indéfini\n");
                        i->interet = SAUT_COND_INDEFINI;
                    }
                    if (cibleAdress < fin && cibleAdress>=debut) {
                        Graphe* t = &pi[cibleAdress - debut];
                        t->VirtualAddrPointee = cibleAdress;
                        t->interet = GO_AND_LEAVE;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, t);
                        if (t->listePeres == NULL) {
                            t->listePeres = newLinkedList();
                        }
                        addFirstLL(t->listePeres, i);
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                        if (iniAdress + len < fin) {
                            Graphe* s = &pi[iniAdress + len - debut];
                            s->VirtualAddrPointee = iniAdress + len;
                            s->interet = GO_AND_LEAVE;
                            addFirstLL(i->listeFils, s);
                            if (s->listePeres == NULL) {
                                s->listePeres = newLinkedList();
                            }
                            addFirstLL(s->listePeres, i);
                            addFirstLL(pileAppel, (void *) IP); // on empile
                        } else {
                            pushlog(fichierlog, "WARNING : un saut conditionnel est dernière instruction du bloc\n");
                            i->interet = SAUT_COND_FIN_BLOC;
                        }
                    } else if(iniAdress + len < fin){
                        if (cibleAdress != 0) {
                            pushlog(fichierlog, "WARNING : un saut conditionnel essai de sortir du bloc\n");
                            i->interet = SAUT_COND_OUT_OF_BLOCK;
                        }
                        Graphe* s = &pi[iniAdress + len - debut];
                        s->VirtualAddrPointee = iniAdress + len;
                        s->interet = GO_AND_LEAVE;
                        i->listeFils = newLinkedList();
                        addFirstLL(i->listeFils, s);
                        if (s->listePeres == NULL) {
                            s->listePeres = newLinkedList();
                        }
                        addFirstLL(s->listePeres, i);
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {
                        pushlog(fichierlog, "WARNING : un saut conditionnel n'a pas de fils\n");
                        i->interet = SAUT_COND_TERMINAL;
                        stop = depilage(prog, pileAppel, fichierlog);
                    }
                    break;
                   
            }
        }
        prog->SecurityBlock = (unsigned int) (fin - prog->VirtualAddr);
    }
    pushlog(fichierlog, "fin de la lecture\n");
    closeFichier(fichierlog);
}


/**
 * Cette fonction est reccursive.
 *
 * au moment ou un fait la reccursion, il faut verifier que on ne passe pas 
 * une adresse en dehors du bloc.
 * 
 * le graphe passe est l un des fils de l iteration precedente
 * 
 * prog et g designent la meme chose
 * 
 * dans tous les cas, g sera un pi ou le debut du programme
 *
 * TODO :   ENCORE UN TRUC DE POURRI (ne termine pas) : OK
 * TODO :   on dirait que dans le cas où g n'est pas le départ d'une flèche
 *          alors la tete se retrouve plus loin de prog->VirtualAdress (? OK)
 * TODO :   Améliorer la gestion des saut non définis. Il faut que les jumps inconditionnels 
 *          dans cette situation soient la fin d'un thread (OK)
 * TODO :   Traiter l'ensemble des appels reccursifs des instructions de saut
            en un seul cas
 *
 * Si on decide que on ne fait pas une fleche depuis les ret, il suffit d ajouter "ret " au "hlt "
 */

static void assembleGraphe_aux(DISASM* prog, Graphe* g, Fichier* fichierlog){
    char temp[MAX_BUFFER];
    
    Disasm(prog);
    sprintf(temp, "\ng : %lx : %lx (%s), interet : %d\n",g->VirtualAddrLue, prog->VirtualAddr, prog->Instruction.Mnemonic, g->interet);
    pushlog(fichierlog, temp);
    
    if (g->VirtualAddrLue == 0) {
        pushlog(fichierlog, "le graphe contient un etat qui n'a pas pas été lu lors de la pré-lecture\n");
        exit(EXIT_FAILURE);
    }
    
    /*==================================== CAS D'ARRET ==================================*/
    if (g->assemble) {
        pushlog(fichierlog, "deja assemble\n");
        return;
    }
    
    /* les autres cas traitent des instructions terminales */
    
    if (g->typeLiaison == FIN) {
        g->assemble = 1;
        pushlog(fichierlog, "un hlt\n");
        if (g->listeFils != NULL) {
            pushlog(fichierlog, "ERREUR : une instruction terminale a des fils\n");
            exit(EXIT_FAILURE);
            //terminateLinkedList(g->listeFils); // on ne souhaite pas détruire les éléments de la liste
            //g->listeFils = NULL;
        }
        return;
    }
    if (g->typeLiaison == RET) {
        g->assemble = 1;
        pushlog(fichierlog, "un ret\n");
        if (g->listeFils != NULL) {
            pushlog(fichierlog, "ERREUR : une instruction terminale a des fils\n");
            exit(EXIT_FAILURE);
            //terminateLinkedList(g->listeFils); // on ne souhaite pas détruire les éléments de la liste
            //g->listeFils = NULL;
        }
        return;
    }
    if (g->interet == OPCODE_INCONNU || g->interet == DEPASSEMENT_BLOC) {
        g->assemble = 1;
        pushlog(fichierlog, "WARNING : une instruction erronée\n");
        if (g->listeFils != NULL) {
            pushlog(fichierlog, "une instruction terminale a des fils\n");
            exit(EXIT_FAILURE);
            //terminateLinkedList(g->listeFils); // on ne souhaite pas détruire les éléments de la liste
            //g->listeFils = NULL;
        }
        return;
    }
    if (g->listeFils == NULL) { // normalement seul les ret et les hlt n'ont pas de fils
        g->assemble = 1;
        if (g->typeLiaison == CALL) {
            pushlog(fichierlog, "WARNING : call sans fils\n");
        }
        if (g->typeLiaison == JUMP_INCOND) {
            
            pushlog(fichierlog, "WARNING : jmp sans fils\n");
        }
        if (g->typeLiaison == JUMP_COND) {
            pushlog(fichierlog, "WARNING : jne sans fils\n");
        }
        if (g->typeLiaison == TERMINAISON) {
            pushlog(fichierlog, "WARNING : l'instruction suivante sort du bloc\n");
        }
        return;
    }
    
    /*===================================================================================*/
    
     g->assemble = 1;
     
    /*================================ APPELS RECCURSIFS ===============================*/
    
    if (g->typeLiaison != TERMINAISON) {
        if (g->typeLiaison == CALL) {
            pushlog(fichierlog, "un call\n");
        }
        if (g->typeLiaison == JUMP_COND) {
            pushlog(fichierlog, "un jump\n");
        }
        if (g->typeLiaison == JUMP_INCOND) {
            pushlog(fichierlog, "un jne\n");
        }
        if (g->listeFils->longueur == 0) {
            pushlog(fichierlog, "un saut n'a pas de fils et l'assemblage ne s'est pas arrété\n");
            exit(EXIT_FAILURE);
        }
        
        unsigned long EIPini = prog->EIP;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        unsigned long VirtualAddrIni = prog->VirtualAddr;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        long secuIni = prog->SecurityBlock;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            
            
            unsigned long addrCible = etatCible->VirtualAddrLue; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            long ecart = addrCible - VirtualAddrIni; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->EIP = EIPini + ecart; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->VirtualAddr = addrCible; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->SecurityBlock = (int) (secuIni - ecart); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            
            assembleGraphe_aux(prog, etatCible, fichierlog);
            tete = tete->suiv;
        }
        return;
    }
    
    
    /* on fait maintenant le cas ou g n est pas le depart d une fleche*/
    /* qui n'est pas une instruction terminale */
    
    pushlog(fichierlog, "un non depart d un fleche\n");
       
    int len = Disasm2(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
    
    if (g->listeFils == NULL || g->listeFils->longueur == 0) {
        pushlog(fichierlog, "etat terminal non marqué (interet nul)\n");
        exit(EXIT_FAILURE);
    }
    
    Graphe* tete = g->listeFils->valeur;
    
    LinkedList* filsUnique = newLinkedList();
    addFirstLL(filsUnique, (void*) tete);
    g->listeFils = filsUnique; // on sait que g n est pas de depart d une fleche
    
    len = Disasm(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
    
    while (tete->interet == 0) {
        
        tete->assemble=1; // normlament on pourrait se contenter des la dernière tete
        prog->EIP += len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        prog->VirtualAddr +=len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        prog->SecurityBlock = prog->SecurityBlock - len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        
        if (tete->listeFils == NULL || tete->listeFils->longueur == 0) {
            pushlog(fichierlog, "etat terminal non marqué (interet nul)\n");
            exit(EXIT_FAILURE);
        }
        tete = tete->listeFils->valeur;
        filsUnique->valeur = tete;
        len = Disasm(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
    }
    assembleGraphe_aux(prog, tete, fichierlog);
    return;
}

/**
 * Cette fonction assemble les elements du tableau
 * pour en faire un seul graphe
 */

Graphe* assembleGraphe(desasembleur* desas, Graphe pi[]){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
//    strcat(chemin_log, "/assemblage.log");
    strcat(chemin_log, CHEMIN_LOG_ASSEMBLAGE);
    Fichier* fichierlog = newFichier(chemin_log);
    
    pushlog(fichierlog, "début de l'assemblage\n");
    DISASM* prog = desas->disasm;
    Graphe* g = &pi[prog->VirtualAddr - desas->debutVirtuel]; // la premiere instruction est forcement non vide
    assembleGraphe_aux(prog, g, fichierlog);
    g->assemble=1;
    pushlog(fichierlog, "fin de l'assemblage");
    closeFichier(fichierlog);
    return g;
}

Graphe* ControleFlow3(desasembleur* desas){
    DISASM* prog = desas->disasm;
    unsigned long sb = prog->SecurityBlock;
    unsigned long debutReel = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    unsigned long taille = sb + virtualAddr - desas->debutVirtuel;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    //afficherPI(pi, taille);
    
    prog->EIP = debutReel;
    prog->SecurityBlock = (unsigned int) sb;
    prog->VirtualAddr = virtualAddr;
    Graphe* g = assembleGraphe(desas, pi);
    return g;
}

/*--------------------------------------------------------------------------------------------------*/
/*                                                                                                  */
/*                                           AFFICHAGE                                              */
/*                                                                                                  */
/*--------------------------------------------------------------------------------------------------*/

/**
 * TODO :   Amélioration des couleurs en fonctions des différents cas.
 */

static void afficheCF_aux(Graphe* g){
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
        if (g->interet == OPCODE_INCONNU || g->interet == DEPASSEMENT_BLOC) {
            printf("[style=filled fillcolor=red]");
        }
        if (g->interet < -2) {
            printf("[style=filled fillcolor=orange]");
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

static void enregistreCF_aux(Graphe* g, FILE* graveur){
    if (g->affiche) {
        fprintf(graveur, "\"%lx\";\n", g->VirtualAddrLue);
        return;
    }
    if (g->listeFils == NULL) {
        fprintf(graveur, "\"%lx\"", g->VirtualAddrLue);
        g->affiche = 1;
        if (g->typeLiaison == RET) {
            fprintf(graveur, "[style=filled fillcolor=grey]");
        }
        if (g->interet == OPCODE_INCONNU || g->interet == DEPASSEMENT_BLOC) {
            fprintf(graveur, "[style=filled fillcolor=red]");
        }
        if (g->interet < -2) {
            fprintf(graveur, "[style=filled fillcolor=orange]");
        }
        fprintf(graveur, ";\n");
        return;
    }
    g->affiche = 1;
    LinkedList* tete = g->listeFils;
    int totFils = (int) sizeLL(g->listeFils);
    for (int i = 0; i<totFils; i++) { // on visite tous les fils.
        Graphe* etatCible = tete->valeur;
        fprintf(graveur, "\"%lx\"->\"%lx\"", g->VirtualAddrLue, etatCible->VirtualAddrLue);
        if (g->typeLiaison == CALL) {
            if (etatCible->debutFonction) {
                fprintf(graveur, " [color=red];\n");
            } else {
                fprintf(graveur, ";\n");
            }
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=red]", g->VirtualAddrLue);
        }
        if (g->typeLiaison == JUMP_INCOND) {
            fprintf(graveur, " [color=blue];\n");
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=blue]", g->VirtualAddrLue);
        }
        if (g->typeLiaison == JUMP_COND) {
            fprintf(graveur, " [color=green];\n");
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=green]", g->VirtualAddrLue);
        }
        fprintf(graveur, ";\n");
        enregistreCF_aux(etatCible,graveur);
        tete = tete->suiv;
    }
}

void afficheCF(Graphe* g){
    printf("digraph mon_graphe {\n");
    afficheCF_aux(g);
    printf("}\n");
    return;
}

void enregistreCF(Graphe* g, Fichier* tmp){
    FILE* graveur = ouvrirEcriture(tmp);
    fprintf(graveur, "digraph mon_graphe {\n");
    enregistreCF_aux(g, graveur);
    fprintf(graveur, "}");
    fclose(graveur);
    return;
}

static void afficheListeFils(Graphe* g){
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<g->listeFils->longueur; i++) {
        Graphe* fils = tete->valeur;
        printf(", %lx", fils->VirtualAddrLue);
        tete = tete->suiv;
    }
    
}

static void afficheListePere(Graphe* g){
    LinkedList* tete = g->listePeres;
    for (int i = 0; i<g->listePeres->longueur; i++) {
        Graphe* fils = tete->valeur;
        printf(", %lx", fils->VirtualAddrLue);
        tete = tete->suiv;
    }
    
}

void afficherPI(Graphe* pi, unsigned long taille){
    for (int i = 0; i<taille; i++) {
        printf("0x%lx\tinteret : %d",pi[i].VirtualAddrLue, pi[i].interet);
        if (pi[i].listeFils != NULL) {
            printf("\tfils : ");
            afficheListeFils(&pi[i]);
        }
        if (pi[i].listePeres != NULL) {
            printf("\tpere : ");
            afficheListePere(&pi[i]);
        }
        printf("\n");
    }
}

void afficheCrible(int* crible, unsigned long taille, unsigned long pev){
    for (unsigned long i = 0; i<taille; i++) {
        printf("%lx \t %d\n", pev + i, crible[i]);
    }
}











