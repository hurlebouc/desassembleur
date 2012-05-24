//
//  Graphe.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "Graphe.h"
#include "_macro_Build.h"

Graphe* newGraphe(void){
    Graphe* g = malloc(sizeof(Graphe));
    g->VirtualAddr = 0;
    g->aif = 0;
    g->etat = SANS_INTERET;
    g->typeLiaison = NOEUD_TERMINAISON;
    g->_immediat = NEANT;
    g->tailleInstruction = 0;
    g->recouvert = 0;
    g->listeFils = NULL;
    g->listePeres = NULL;
    g->pool = newProcesseur();
    return g;
}

void terminateNoeud(Graphe* g){
    
    if (g->listePeres != NULL) {
        while (sizeLL(g->listePeres) != 0) {
            Graphe* pere = getFirstLL(g->listePeres);
            removeLink(pere, g);
        }
    }
    
    if (g->listeFils != NULL) {
        while (sizeLL(g->listeFils) != 0) {
            Graphe* fils = getFirstLL(g->listeFils);
            removeLink(g, fils);
        }
    }
    if (g->listeFils != NULL) {
        terminateLinkedList(g->listeFils);
    }
    if (g->listePeres != NULL) {
        terminateLinkedList(g->listePeres);
    }
    terminateProcesseur(g->pool);
    free(g);
}

void terminateGraphe(Graphe* g){
    // Il faut eviter que se faire supprimer comme étant
    // un (arriere petit) fils de ses fils.
    if (g->_immediat == EST_LIBERE) {
        return;
    }
    g->_immediat = EST_LIBERE;
    
    // suppression de la liste des fils de chaque pere
    if (g->listePeres != NULL) {
        while (sizeLL(g->listePeres) != 0) {
            Graphe* pere = getFirstLL(g->listePeres);
            removeLink(pere, g);
        }
    }
    
    // effacement de chaque fils
    /*
     * Rq : la liste des fils de g diminue toute seule car les fils de g s'y 
     * suppriment au fur et à mesure qu'ils s'effacent.
     */
    if (g->listeFils != NULL) {
        while (sizeLL(g->listeFils) != 0) {
            Graphe* fils = getFirstLL(g->listeFils);
            terminateGraphe(fils);
        }
    }
    if (g->listeFils != NULL) {
        terminateLinkedList(g->listeFils);
    }
    if (g->listePeres != NULL) {
        terminateLinkedList(g->listePeres);
    }
    terminateProcesseur(g->pool);
    free(g);
}
/**
 * Cette fonction ne devrait pas être utilisée hors de cette classe car elle
 * ne maintient pas la propriete d'accessibilite de tous les noeuds du graphe.
 * Il vaut mieux lui référer removeLinkRec()
 */

void removeLink(Graphe* pere, Graphe* fils){
    removeElementLL(pere->listeFils, fils);
    removeElementLL(fils->listePeres, pere);
}

void removeLinkRec(Graphe* pere, Graphe* fils){
    removeLink(pere, fils);
    if (sizeLL(fils->listePeres) == 0) {
        while (sizeLL(fils->listeFils) != 0) {
            removeLinkRec(fils, getFirstLL(fils->listeFils));
        }
        terminateNoeud(fils);
    }
}

void addLink(Graphe* pere, Graphe* fils){
    if (pere->listeFils == NULL) {
        pere->listeFils = newLinkedList();
    }
    if (fils->listePeres == NULL) {
        fils->listePeres = newLinkedList();
    }
    addFirstLL(pere->listeFils, fils);
    addFirstLL(fils->listePeres, pere);
}

/*
 * Un noeud inaccessible ne sera pas trouvé
 */

Graphe* getNodeWithVirtualAddr(Graphe* g, uintptr_t va){
    if (g->_immediat == PASSAGE_GET_NODE_WITH_VIRTUALADDR) {
        return NULL;
    }
    uint8_t etatinit = g->_immediat;
    g->_immediat = PASSAGE_GET_NODE_WITH_VIRTUALADDR;
    if(g->VirtualAddr == va){
        g->_immediat = etatinit;
        return g;
    }
    if (g->listeFils == NULL) {
        g->_immediat = etatinit;
        return NULL;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        Graphe* res = getNodeWithVirtualAddr(getFirstLL(tete), va);
        if (res != NULL) {
            g->_immediat = etatinit;
            return res;
        }
        tete=tete->suiv;
    }
    g->_immediat = etatinit;
    return NULL;
}

Graphe* getNodeWithVirtualAddrUnique(Graphe* g, uintptr_t va){
    if (g->_immediat == PASSAGE_GET_NODE_WITH_VIRTUALADDR_U) {
        return NULL;
    }
    g->_immediat = PASSAGE_GET_NODE_WITH_VIRTUALADDR_U;
    if(g->VirtualAddr == va){
        return g;
    }
    if (g->listeFils == NULL) {
        return NULL;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        Graphe* res = getNodeWithVirtualAddr(getFirstLL(tete), va);
        if (res != NULL) {
            return res;
        }
        tete=tete->suiv;
    }
    return NULL;
}

/*------------------------ FONCTIONS DE TRAITEMENT --------------------------*/


static void setRegistre(int i,ARGTYPE*argument,Processeur*proc,Registre**reg) {
    uint16_t loword = argument[i].ArgType;
    int size = argument[i].ArgSize;
    switch (loword) {
        case REG0:
            switch (size) {
                case 64:
                    reg[i] = _RAX;
                    break;
                case 32:
                    reg[i] = _EAX;
                    break;
                case 16:
                    reg[i] = _AX;
                    break;
                case 8:
                    if (argument[i].ArgPosition == HighPosition) {
                        reg[i] = _AH;
                    } else {
                        reg[i] = _AL;
                    }
                    break;
                default:
                    break;
            }
            break;
        case REG1:
            switch (size) {
                case 64:
                    reg[i] = _RCX;
                    break;
                case 32:
                    reg[i] = _ECX;
                    break;
                case 16:
                    reg[i] = _CX;
                    break;
                case 8:
                    if (argument[i].ArgPosition == HighPosition) {
                        reg[i] = _CH;
                    } else {
                        reg[i] = _CL;
                    }
                    break;
                default:
                    break;
            }
            break;
        case REG2:
            switch (size) {
                case 64:
                    reg[i] = _RDX;
                    break;
                case 32:
                    reg[i] = _EDX;
                    break;
                case 16:
                    reg[i] = _DX;
                    break;
                case 8:
                    if (argument[i].ArgPosition == HighPosition) {
                        reg[i] = _DH;
                    } else {
                        reg[i] = _DL;
                    }
                    break;
                default:
                    break;
            }
            break;
        case REG3:
            switch (size) {
                case 64:
                    reg[i] = _RBX;
                    break;
                case 32:
                    reg[i] = _EBX;
                    break;
                case 16:
                    reg[i] = _BX;
                    break;
                case 8:
                    if (argument[i].ArgPosition == HighPosition) {
                        reg[i] = _BH;
                    } else {
                        reg[i] = _BL;
                    }
                    break;
                default:
                    break;
            }
            break;
        case REG4:
            switch (size) {
                case 64:
                    reg[i] = _RSP;
                    break;
                case 32:
                    reg[i] = _ESP;
                    break;
                case 16:
                    reg[i] = _SP;
                    break;
                case 8:
                    reg[i] = _SPL;
                    break;
                default:
                    break;
            }
            break;
        case REG5:
            switch (size) {
                case 64:
                    reg[i] = _RBP;
                    break;
                case 32:
                    reg[i] = _EBP;
                    break;
                case 16:
                    reg[i] = _BP;
                    break;
                case 8:
                    reg[i] = _BPL;
                    break;
                default:
                    break;
            }
            break;
        case REG6:
            switch (size) {
                case 64:
                    reg[i] = _RSI;
                    break;
                case 32:
                    reg[i] = _ESI;
                    break;
                case 16:
                    reg[i] = _SI;
                    break;
                case 8:
                    reg[i] = _SIL;
                    break;
                default:
                    break;
            }
            break;
        case REG7:
            switch (size) {
                case 64:
                    reg[i] = _RDI;
                    break;
                case 32:
                    reg[i] = _EDI;
                    break;
                case 16:
                    reg[i] = _DI;
                    break;
                case 8:
                    reg[i] = _DIL;
                    break;
                default:
                    break;
            }
            break;
        case REG8:
            switch (size) {
                case 64:
                    reg[i] = _R8;
                    break;
                case 32:
                    reg[i] = _R8D;
                    break;
                case 16:
                    reg[i] = _R8W;
                    break;
                case 8:
                    reg[i] = _R8B;
                    break;
                default:
                    break;
            }
            break;
        case REG9:
            switch (size) {
                case 64:
                    reg[i] = _R9;
                    break;
                case 32:
                    reg[i] = _R9D;
                    break;
                case 16:
                    reg[i] = _R9W;
                    break;
                case 8:
                    reg[i] = _R9B;
                    break;
                default:
                    break;
            }
            break;
        case REG10:
            switch (size) {
                case 64:
                    reg[i] = _R10;
                    break;
                case 32:
                    reg[i] = _R10D;
                    break;
                case 16:
                    reg[i] = _R10W;
                    break;
                case 8:
                    reg[i] = _R10B;
                    break;
                default:
                    break;
            }
            break;
        case REG11:
            switch (size) {
                case 64:
                    reg[i] = _R11;
                    break;
                case 32:
                    reg[i] = _R11D;
                    break;
                case 16:
                    reg[i] = _R11W;
                    break;
                case 8:
                    reg[i] = _R11B;
                    break;
                default:
                    break;
            }
            break;
        case REG12:
            switch (size) {
                case 64:
                    reg[i] = _R12;
                    break;
                case 32:
                    reg[i] = _R12D;
                    break;
                case 16:
                    reg[i] = _R12W;
                    break;
                case 8:
                    reg[i] = _R12B;
                    break;
                default:
                    break;
            }
            break;
        case REG13:
            switch (size) {
                case 64:
                    reg[i] = _R13;
                    break;
                case 32:
                    reg[i] = _R13D;
                    break;
                case 16:
                    reg[i] = _R13W;
                    break;
                case 8:
                    reg[i] = _R13B;
                    break;
                default:
                    break;
            }
            break;
        case REG14:
            switch (size) {
                case 64:
                    reg[i] = _R14;
                    break;
                case 32:
                    reg[i] = _R14D;
                    break;
                case 16:
                    reg[i] = _R14W;
                    break;
                case 8:
                    reg[i] = _R14B;
                    break;
                default:
                    break;
            }
            break;
        case REG15:
            switch (size) {
                case 64:
                    reg[i] = _R15;
                    break;
                case 32:
                    reg[i] = _R15D;
                    break;
                case 16:
                    reg[i] = _R15W;
                    break;
                case 8:
                    reg[i] = _R15B;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/**
 * Cette fonction lit l'instruction du noeud g à partir du (nouveau) pool de 
 * son père
 * Ici g ne sert qu'à donner l'instruction (on utilise pas son pool)
 */

static void setPool(Graphe* g, Processeur* newPool) {
    DISASM* disasm = newDisasm();
    disasm->EIP = g->aif;
    disasm->VirtualAddr = g->VirtualAddr;
    int len = Disasm(disasm);
    
    INSTRTYPE instr = disasm->Instruction;
    
    Instruction* instruction = NULL;
    Registre* reg[] = {NULL, NULL, NULL};
    
    /* initialisation de l'instruction */
    switch (instr.Opcode) {
        case 0x00:
            //instruction = init_add_EbGb();
            instruction = init_add();
            break;
        case 0x01:
            //instruction = init_add_EbGb();
            instruction = init_add();
            break;
            
        default:
            //            printf("litInstruction() : opcode inconnu\n");
            //            exit(EXIT_FAILURE);
            break;
    }
    
    ARGTYPE argument[] = {
        disasm->Argument1,
        disasm->Argument2,
        disasm->Argument3
    };
    
    uint8_t aSuppr[]={0,0,0};
    
    /* initialisation des des arguments*/
    for (int i = 0; i<3; i++) {
        uint32_t hiword = argument[i].ArgType & 0xffff0000;
        switch (hiword) {
            case NO_ARGUMENT:
                ;
                break;
            case REGISTER_TYPE + MMX_REG:
                ;
                break;
            case REGISTER_TYPE + GENERAL_REG:
                setRegistre(i, argument, newPool, reg);
                break;
            case REGISTER_TYPE + FPU_REG:
                ;
                break;
            case REGISTER_TYPE + SSE_REG:
                ;
                break;
            case REGISTER_TYPE + CR_REG:
                ;
                break;
            case REGISTER_TYPE + DR_REG:
                ;
                break;
            case REGISTER_TYPE + SPECIAL_REG:
                ;
                break;
            case REGISTER_TYPE + MEMORY_MANAGEMENT_REG:
                ;
                break;
            case REGISTER_TYPE + SEGMENT_REG:
                ;
                break;
            case MEMORY_TYPE:
                ;
                break;
            case CONSTANT_TYPE + RELATIVE_:
                ;
                break;
            case CONSTANT_TYPE + ABSOLUTE_:
                reg[i] = newRegistreFeuille(64, disasm->Instruction.Immediat);
                // c'est un nouveau registre qu'il faut supprimer.
                aSuppr[i] = 1;
                break;
                
            default:
                break;
        }
    }
    if (instruction != NULL) {
        do_instr(instruction, reg[0], reg[1], reg[2], len, newPool);
        terminateInstruction(instruction);
    }
    for (int i = 0; i<3; i++) {
        if (aSuppr[i]) {
            terminateRegistre(reg[i]);
        }
    }
    
    free(disasm);
}

/**
 * Cette version de l'algorithme de propagation des constantes est une 
 * optimisation mémoire de l'algo de Kildall. Il se fait en espace constant.
 */

static void optimizePool_aux(Graphe* g, const Processeur* initialPool, Fichier* fichierlog, char temp[MAX_BUFFER]){
    sprintf(temp, "optimise 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
    
    Processeur* copyPool = newProcesseurCopy(initialPool);
    setPool(g, copyPool);
    int inc = incluDans(g->pool, copyPool);
    if (inc !=NON_INCLUS){
        terminateProcesseur(copyPool);
        sprintf(temp,"fin de 0x%lx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, copyPool); // l'intercection est dans g->pool
    terminateProcesseur(copyPool);
    if (g->listeFils == NULL) {
        sprintf(temp,"fin de 0x%lx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    pushlog(fichierlog, temp);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        optimizePool_aux(tete->valeur, g->pool, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp,"fin de 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/**
 * Cette version de l'algorithme de propagation des constantes est une 
 * optimisation mémoire de l'algo de Kildall. Il se fait en espace constant.
 */

static void optimizePool_aux2(Graphe* g, const Processeur* initialPool, Fichier* fichierlog, char temp[MAX_BUFFER]){
    sprintf(temp, "optimise 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
    
    Processeur* copyPool = newProcesseurCopy(initialPool);
    setPool(g, copyPool);
    int inc = incluDans(g->pool, copyPool);
    if (inc !=NON_INCLUS){
        terminateProcesseur(copyPool);
        sprintf(temp,"fin de 0x%lx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, copyPool); // l'intercection est dans g->pool
    terminateProcesseur(copyPool);
    if (g->listeFils == NULL) {
        sprintf(temp,"fin de 0x%lx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    pushlog(fichierlog, temp);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        optimizePool_aux2(tete->valeur, g->pool, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp,"fin de 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/**
 * Cette version de propagation des constante respecte l'agorithme de Kildall
 * (a utiliser dans optimizePool (version 1)
 */

static void optimizePool_aux_kildall(Graphe* g, const Processeur* initialPool, Fichier* fichierlog, char temp[MAX_BUFFER]){
    sprintf(temp, "optimise 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
    
    int inc = incluDans(g->pool, initialPool);
    if (inc !=NON_INCLUS){
        sprintf(temp,"fin de 0x%lx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, initialPool); // l'intercection est dans g->pool
    if (g->listeFils == NULL) {
        sprintf(temp,"fin de 0x%lx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    Processeur* newPool = newProcesseurCopy(g->pool);
    setPool(g, newPool); // ici newPool est modifie par g
    int l = sizeLL(g->listeFils);
    pushlog(fichierlog, temp);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        optimizePool_aux_kildall(tete->valeur, newPool, fichierlog, temp);
        tete = tete->suiv;
    }
    terminateProcesseur(newPool);
    sprintf(temp,"fin de 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/*
 * Il ne faut pas que initialPool soit modifié car sinon on modifierait le 
 * pool du père
 */

void optimizePool(Graphe* g, const Processeur* initialPool){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
    strcat(chemin_log, CHEMIN_LOG_OPTIMISATION);
    Fichier* fichierlog = newFichier(chemin_log);    
    char temp[MAX_BUFFER];
    
    optimizePool_aux(g, initialPool, fichierlog, temp);
    
    closeFichier(fichierlog);
}

void optimizePool2(Graphe* g, const Processeur* initialPool){
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
    strcat(chemin_log, CHEMIN_LOG_OPTIMISATION);
    Fichier* fichierlog = newFichier(chemin_log);    
    char temp[MAX_BUFFER];
    
    int inc = incluDans(g->pool, initialPool);
    if (inc !=NON_INCLUS){
        sprintf(temp,"fin de 0x%lx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    
    inter(g->pool, initialPool); // l'intercection est dans g->pool
    
    if (g->listeFils == NULL) {
        sprintf(temp,"fin de 0x%lx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    pushlog(fichierlog, temp);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        optimizePool_aux2(tete->valeur, g->pool, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp,"fin de 0x%lx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
    
    closeFichier(fichierlog);
}

void debranchage(Graphe* g){
    DISASM* disasm = newDisasm();
    disasm->EIP = g->aif;
    disasm->VirtualAddr = g->VirtualAddr;
    Disasm(disasm);
    
    int branch = disasm->Instruction.BranchType;
    
    switch (branch) {
        case JO:
            ;
            break;
            
        default:
            break;
    }
}