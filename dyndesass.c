#include "dyndesass.h"

Graphe* GRAPHE_RECCOUVERT;

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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                            PRE-TRAITEMENT                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

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

static Graphe* initGraph(Graphe* pi[], unsigned long index){
    if (pi[index] == NULL) {
        pi[index] = newGraphe();
    }
    if (pi[index]->recouvert == EST_RECOUVERT) {
        pi[index] = newGraphe();
        pi[index]->recouvert = EST_RECOUVERT;
    }
    return pi[index];
}

Graphe* buildGraphe(Desasembleur* desas, Graphe* pi[]){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
    strcat(chemin_log, CHEMIN_LOG_FERMETURE);
    Fichier* fichierlog = newFichier(chemin_log);    
    char temp[MAX_BUFFER];
    
    GRAPHE_RECCOUVERT = newGraphe();
    GRAPHE_RECCOUVERT->recouvert = EST_RECOUVERT;
    
    DISASM* prog = desas->disasm;
    LinkedList* pileAppel = newLinkedList();
    int stop = 0;
    unsigned long debut = desas->debutVirtuel;
    unsigned long fin = prog->SecurityBlock + prog->VirtualAddr;
    
    /* Mémorisation des paramètres initiaux */
    
    unsigned long virtualAddr_init = prog->VirtualAddr;
    
    while (!stop) {
        
        //int len = Disasm(prog);
        int len = desassemble(desas);
        int brancheType = prog->Instruction.BranchType;
        unsigned long iniAdress = prog->VirtualAddr;
        unsigned long cibleAdress = prog->Instruction.AddrValue;
        unsigned long IP = iniAdress + len;
        sprintf(temp,  "0x%lx \t %s \t (0x%lx) (op : %x)\n", iniAdress, prog->CompleteInstr, cibleAdress, prog->Instruction.Opcode);
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
        Graphe* i = initGraph(pi, iniAdress - debut);
//        Graphe* i = &pi[iniAdress - debut];
        i->VirtualAddr = iniAdress;
        i->tailleInstruction = len;
        i->aif = prog->EIP;
        for (int k = 1; k<len; k++) {
            if (iniAdress + k < fin) {
                pi[iniAdress + k  - debut] = GRAPHE_RECCOUVERT;
//                pi[iniAdress + k  - debut].recouvert = EST_RECOUVERT;
            }
        }
                
        if (len == UNKNOWN_OPCODE) {
            pushlog(fichierlog, "WARNING : le desassembleur a rencontré un opcode inconnu\n");
            i->interet = OPCODE_INCONNU;
            i->etat = EST_LU;
            stop = depilage(prog, pileAppel, fichierlog);
            
        } else if (len == OUT_OF_BLOCK) {
            pushlog(fichierlog, "WARNING : le désassembleur a rencontré un depassement de bloc lors de la lecture d'une instruction\n");
            i->interet = DEPASSEMENT_BLOC;
            i->etat = EST_LU;
            stop = depilage(prog, pileAppel, fichierlog);
            
        } else if (i->etat == EST_LU){ // si on est deja passé par là
            stop = depilage(prog, pileAppel, fichierlog); 
                        
        } else {
            i->etat = EST_LU;
            switch (brancheType) {
                    
                case CallType:
                    i->interet = GO_AND_LEAVE;
                    i->typeLiaison = CALL;
                    if (cibleAdress == 0) {
                        pushlog(fichierlog, "WARNING : call indéfini\n");
                        i->interet = CALL_INDEFINI;
                    }
                    if (cibleAdress < fin && cibleAdress >= debut) {
                        Graphe* t = initGraph(pi, cibleAdress - debut);
//                        Graphe* t = &pi[cibleAdress - debut];
//                        t->VirtualAddrPointee = cibleAdress;
//                        t->interet = GO_AND_LEAVE;
                        t->interet = DEBUT_FONCTION;
//                        t->debutFonction = 1;
                        addLink(i, t);
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                        if (iniAdress + len < fin) {
                            Graphe* s = initGraph(pi, iniAdress + len - debut);
//                            Graphe* s = &pi[iniAdress + len - debut];
//                            s->VirtualAddrPointee = iniAdress + len;
                            s->interet = GO_AND_LEAVE;
                            addLink(i, s);
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
                        Graphe* s = initGraph(pi, iniAdress + len - debut);
//                        Graphe* s = &pi[iniAdress + len - debut];
//                        s->VirtualAddrPointee = iniAdress + len;
                        s->interet = GO_AND_LEAVE;
                        addLink(i, s);
                        prog->VirtualAddr += len;
                        prog->EIP += len;
                    } else {     // cas où un call est en fin de bloc et donc l'appel est indéfini
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
                        Graphe* t = initGraph(pi, cibleAdress - debut);
//                        Graphe* t = &pi[cibleAdress - debut];
//                        t->VirtualAddrPointee = cibleAdress;
                        t->interet = GO_AND_LEAVE;
                        addLink(i, t);
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
                        Graphe* s = initGraph(pi, iniAdress + len - debut);
//                        Graphe* s = &pi[iniAdress + len - debut];
//                        s->VirtualAddrPointee = iniAdress + len;
                        addLink(i, s);
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
                        Graphe* t = initGraph(pi, cibleAdress - debut);
//                        Graphe* t = &pi[cibleAdress - debut];
//                        t->VirtualAddrPointee = cibleAdress;
                        t->interet = GO_AND_LEAVE;
                        addLink(i, t);
                        prog->VirtualAddr = cibleAdress;
                        prog->EIP += cibleAdress - (long) iniAdress;
                        if (iniAdress + len < fin) {
                            Graphe* s = initGraph(pi, iniAdress + len - debut);
//                            Graphe* s = &pi[iniAdress + len - debut];
//                            s->VirtualAddrPointee = iniAdress + len;
                            s->interet = GO_AND_LEAVE;
                            addLink(i, s);
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
                        Graphe* s = initGraph(pi, iniAdress + len - debut);
//                        Graphe* s = &pi[iniAdress + len - debut];
//                        s->VirtualAddrPointee = iniAdress + len;
                        s->interet = GO_AND_LEAVE;
                        addLink(i, s);
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
    terminateLinkedList(pileAppel);
    Graphe* g = initGraph(pi, virtualAddr_init - debut);
//    Graphe* g = &pi[virtualAddr_init - debut];
    return g;
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

static void simplifieGraphe_aux(DISASM* prog, Graphe* g, Fichier* fichierlog){
    char temp[MAX_BUFFER];
    
#ifdef DEBUG_MODE
    Disasm(prog);
    sprintf(temp, "\ng : %lx : %lx (%s), interet : %d\n",g->VirtualAddr, prog->VirtualAddr, prog->Instruction.Mnemonic, g->interet);
    pushlog(fichierlog, temp);
    
    if (g->VirtualAddr != prog->VirtualAddr) {
        sprintf(temp, "le graphe et le programme en mémoire sont différents");
        pushlog(fichierlog, temp);
        exit(EXIT_FAILURE);
    }
#endif

    DISASM prog2;
    (void) memset(&prog2, 0, sizeof (DISASM));
    prog2.EIP = g->aif;
    prog2.VirtualAddr = g->VirtualAddr;
    prog2.SecurityBlock = g->tailleInstruction;
    prog2.Archi = ARCHI_PROC;
    prog2.Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    Disasm(&prog2);
    sprintf(temp, "%lx \t %s\n",g->VirtualAddr, prog2.CompleteInstr);
    pushlog(fichierlog, temp);
        
    if (g->VirtualAddr == 0) {
        pushlog(fichierlog, "le graphe contient un etat qui n'a pas pas été lu lors de la pré-lecture\n");
        exit(EXIT_FAILURE);
    }
    
    /*==================================== CAS D'ARRET ==================================*/
    if (g->etat == EST_ASSEMBLE) {
        pushlog(fichierlog, "deja assemble\n");
        return;
    }
    
    /* les autres cas traitent des instructions terminales */
    
    if (g->typeLiaison == FIN) {
        g->etat = EST_ASSEMBLE;
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
        g->etat = EST_ASSEMBLE;
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
        g->etat = EST_ASSEMBLE;
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
        g->etat = EST_ASSEMBLE;
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
    
     g->etat = EST_ASSEMBLE;
     
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
        
#ifdef DEBUG_MODE
        unsigned long EIPini = prog->EIP;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        unsigned long VirtualAddrIni = prog->VirtualAddr;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        long secuIni = prog->SecurityBlock;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
        
        LinkedList* tete = g->listeFils;
        int totFils = (int) sizeLL(g->listeFils);
        for (int i = 0; i<totFils; i++) { // on visite tous les fils.
            Graphe* etatCible = tete->valeur;
            
#ifdef DEBUG_MODE
            unsigned long addrCible = etatCible->VirtualAddr; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            long ecart = addrCible - VirtualAddrIni; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->EIP = EIPini + ecart; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->VirtualAddr = addrCible; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
            prog->SecurityBlock = (int) (secuIni - ecart); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
            
            simplifieGraphe_aux(prog, etatCible, fichierlog);
            tete = tete->suiv;
        }
        return;
    }
    
    
    /* on fait maintenant le cas ou g n est pas le depart d une fleche*/
    /* qui n'est pas une instruction terminale */
    
    pushlog(fichierlog, "un non depart d un fleche\n");
       
#ifdef DEBUG_MODE
    int len = Disasm2(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
    
    if (g->listeFils == NULL || g->listeFils->longueur == 0) {
        pushlog(fichierlog, "etat terminal non marqué (interet nul)\n");
        exit(EXIT_FAILURE);
    }
    
    LinkedList* filsUnique = g->listeFils;
    Graphe* fils = filsUnique->valeur;
    Graphe* nouvFils = fils;
    
//    LinkedList* filsUnique = newLinkedList();
//    addFirstLL(filsUnique, (void*) tete);
//    g->listeFils = filsUnique; // on sait que g n est pas de depart d une fleche
    
#ifdef DEBUG_MODE
    len = Disasm(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
    
    while (fils->interet == 0) {
        pushlog(fichierlog, "simplification d'un fils\n");
        
#ifdef DEBUG_MODE
//        tete->etat=EST_ASSEMBLE; // normlament on pourrait se contenter des la dernière tete
        prog->EIP += len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        prog->VirtualAddr +=len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
        prog->SecurityBlock = prog->SecurityBlock - len; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
        
        if (fils->listeFils == NULL || fils->listeFils->longueur == 0) {
            pushlog(fichierlog, "etat terminal non marqué (interet nul)\n");
            exit(EXIT_FAILURE);
        }
        
        nouvFils = getFirstLL(fils->listeFils);
        
//        removeLink(g, fils);
//        removeLink(fils, nouvFils);
        terminateNoeud(fils);
        addLink(g, nouvFils);
        
//        filsUnique->valeur = nouvFils;
        
        fils = nouvFils;
        
        
#ifdef DEBUG_MODE
        len = Disasm(prog); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! A SUPPRIMER
#endif
    }
    simplifieGraphe_aux(prog, nouvFils, fichierlog);
//    simplifieGraphe_aux(prog, fils, fichierlog);
    return;
}

/**
 * Cette fonction assemble les elements du tableau
 * pour en faire un seul graphe
 */

Graphe* simplifieGraphe(Desasembleur* desas, Graphe* g){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
    strcat(chemin_log, CHEMIN_LOG_ASSEMBLAGE);
    Fichier* fichierlog = newFichier(chemin_log);
    
    pushlog(fichierlog, "début de l'assemblage\n");
    DISASM* prog = desas->disasm;
    simplifieGraphe_aux(prog, g, fichierlog);
    g->etat=EST_ASSEMBLE;
    pushlog(fichierlog, "fin de l'assemblage");
    closeFichier(fichierlog);
    return g;
}

Graphe* ControleFlow_entier(Desasembleur* desas){
    DISASM* prog = desas->disasm;
    unsigned long sb = prog->SecurityBlock;
    unsigned long virtualAddr = prog->VirtualAddr;
    unsigned long taille = sb + virtualAddr - desas->debutVirtuel;
    Graphe** pi = calloc(sizeof(Graphe*),taille);
    Graphe* g = buildGraphe(desas, pi);
    free(pi);
    return g;
}

Graphe* ControleFlow_simplifie(Desasembleur* desas){
    DISASM* prog = desas->disasm;
    unsigned long sb = prog->SecurityBlock;
    unsigned long debutReel = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    unsigned long taille = sb + virtualAddr - desas->debutVirtuel;
    Graphe** pi = calloc(sizeof(Graphe*),taille);
    Graphe* g = buildGraphe(desas, pi);
    free(pi);
    
    prog->EIP = debutReel;
    prog->SecurityBlock = (unsigned int) sb;
    prog->VirtualAddr = virtualAddr;
    simplifieGraphe(desas, g);
    return g;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                AFFICHAGE                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/**
 * TODO :   Amélioration des couleurs en fonctions des différents cas.
 */

static void afficheGraphe_aux(Graphe* g){
    if (g->etat == EST_AFFICHE) {
        printf("\"%lx\";\n", g->VirtualAddr);
        return;
    }
    if (g->listeFils == NULL) {
        printf("\"%lx\"", g->VirtualAddr);
        g->etat = EST_AFFICHE;
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
    g->etat = EST_AFFICHE;
    LinkedList* tete = g->listeFils;
    int totFils = (int) sizeLL(g->listeFils);
    for (int i = 0; i<totFils; i++) { // on visite tous les fils.
        Graphe* etatCible = tete->valeur;
        printf("\"%lx\"->\"%lx\"", g->VirtualAddr, etatCible->VirtualAddr);
        if (g->typeLiaison == CALL) {
            if (etatCible->interet == DEBUT_FONCTION) {
                printf(" [color=red];\n");
            } else {
                printf(";\n");
            }
            printf("\"%lx\" [style=filled fillcolor=red]", g->VirtualAddr);
        }
        if (g->typeLiaison == JUMP_INCOND) {
            printf(" [color=blue];\n");
            printf("\"%lx\" [style=filled fillcolor=blue]", g->VirtualAddr);
        }
        if (g->typeLiaison == JUMP_COND) {
            printf(" [color=green];\n");
            printf("\"%lx\" [style=filled fillcolor=green]", g->VirtualAddr);
        }
        printf(";\n");
        afficheGraphe_aux(etatCible);
        tete = tete->suiv;
    }
}

static void enregistreGraphe_aux(Graphe* g, FILE* graveur){
    if (g->etat == EST_AFFICHE) {
        fprintf(graveur, "\"%lx\";\n", g->VirtualAddr);
        return;
    }
    if (g->listeFils == NULL) {
        fprintf(graveur, "\"%lx\"", g->VirtualAddr);
        g->etat = EST_AFFICHE;
        if (g->typeLiaison == RET) {
            fprintf(graveur, "[style=filled fillcolor=grey]");
        }
        if (g->interet == OPCODE_INCONNU || g->interet == DEPASSEMENT_BLOC) {
            fprintf(graveur, "[style=filled fillcolor=red]");
        }
        if (g->interet < DEPASSEMENT_BLOC) {
            fprintf(graveur, "[style=filled fillcolor=orange]");
        }
        fprintf(graveur, ";\n");
        return;
    }
    g->etat = EST_AFFICHE;
    LinkedList* tete = g->listeFils;
    int totFils = (int) sizeLL(g->listeFils);
    for (int i = 0; i<totFils; i++) { // on visite tous les fils.
        Graphe* etatCible = tete->valeur;
        fprintf(graveur, "\"%lx\"->\"%lx\"", g->VirtualAddr, etatCible->VirtualAddr);
        if (g->typeLiaison == CALL) {
            if (etatCible->VirtualAddr != g->VirtualAddr + g->tailleInstruction) {
                fprintf(graveur, " [color=red];\n");
            } else {
                fprintf(graveur, ";\n");
            }
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=red]", g->VirtualAddr);
        }
        if (g->typeLiaison == JUMP_INCOND) {
            fprintf(graveur, " [color=blue];\n");
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=blue]", g->VirtualAddr);
        }
        if (g->typeLiaison == JUMP_COND) {
            if (etatCible->VirtualAddr != g->VirtualAddr + g->tailleInstruction) {
                fprintf(graveur, " [color=green];\n");
            } else {
                fprintf(graveur, ";\n");
            }
//            fprintf(graveur, " [color=green];\n");
            fprintf(graveur, "\"%lx\" [style=filled fillcolor=green]", g->VirtualAddr);
        }
        fprintf(graveur, ";\n");
        enregistreGraphe_aux(etatCible,graveur);
        tete = tete->suiv;
    }
}

void afficheGraphe(Graphe* g){
    printf("digraph mon_graphe {\n");
    afficheGraphe_aux(g);
    printf("}\n");
    return;
}

void enregistreGraphe(Graphe* g, Fichier* tmp){
    FILE* graveur = ouvrirEcriture(tmp);
    fprintf(graveur, "digraph mon_graphe {\n");
    enregistreGraphe_aux(g, graveur);
    fprintf(graveur, "}");
    fclose(graveur);
    return;
}

static void afficheListeFils(Graphe* g){
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<g->listeFils->longueur; i++) {
        Graphe* fils = tete->valeur;
        printf(", %lx", fils->VirtualAddr);
        tete = tete->suiv;
    }
    
}

static void afficheListePere(Graphe* g){
    LinkedList* tete = g->listePeres;
    for (int i = 0; i<g->listePeres->longueur; i++) {
        Graphe* fils = tete->valeur;
        printf(", %lx", fils->VirtualAddr);
        tete = tete->suiv;
    }
    
}

void afficherPI(Graphe* pi, unsigned long taille){
    for (int i = 0; i<taille; i++) {
        printf("0x%lx\tinteret : %d",pi[i].VirtualAddr, pi[i].interet);
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











