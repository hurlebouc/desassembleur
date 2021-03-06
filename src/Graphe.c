//
//  Graphe.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "Graphe.h"
#include "_macro_Build.h"
#include <time.h>

Graphe* newGraphe(void) {
    Graphe* g = malloc(sizeof (Graphe));
    g->VirtualAddr = 0;
    g->aif = 0;
    g->etat = SANS_INTERET;
    g->typeLiaison = NOEUD_TERMINAISON;
    g->_immediat = NEANT;
    g->tailleInstruction = 0;
    g->recouvert = 0;
    g->listeFils = NULL;
    g->listePeres = NULL;
    g->pool = newProcesseur(TAILLE_MEMOIRE);
    g->pool->delta = DELTA_LEVE;
    return g;
}

static void reinitPools_aux(Graphe* g, int8_t marqueur){
    if (g->_immediat == marqueur) {
        return;
    }
    g->_immediat = marqueur;
    g->pool->delta = DELTA_LEVE;
    if (g->listeFils == NULL) {
        return;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i<l; i++) {
        reinitPools_aux(tete->valeur, marqueur);
        tete = tete->suiv;
    }
}

static void reinitPools(Graphe* g){
    srand (time(NULL));
    int8_t marqueur = rand();
    while (marqueur == g->_immediat) {
        marqueur = rand();
    }
    reinitPools_aux(g, marqueur);
}

/*!
 * Efface du graphe une liaison de succession.
 *
 * Cette fonction ne devrait pas être utilisée hors de cette classe car elle
 * ne maintient pas la propriete d'accessibilite de tous les noeuds du 
 * graphe.
 * Il vaut mieux lui référer @see removeLinkRec()
 *
 * @param pere Noeud précédent dont on enlève le noeud fils de ses succésseurs
 * @param fils Noeud suivant dont on enlève le noeud père de ses prédécésseurs
 */

static void removeLink(Graphe* pere, Graphe* fils) {
    removeElementLL(pere->listeFils, fils);
    removeElementLL(fils->listePeres, pere);
}

static void terminateNoeud(Graphe* g) {

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

void terminateGraphe(Graphe* g) {
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

void removeLinkRec(Graphe* pere, Graphe* fils) {
    removeLink(pere, fils);
    if (sizeLL(fils->listePeres) == 0) {
        while (fils->listeFils != NULL && sizeLL(fils->listeFils) != 0) {
            removeLinkRec(fils, getFirstLL(fils->listeFils));
        }
        terminateNoeud(fils);
    }
}

void addLink(Graphe* pere, Graphe* fils) {
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

Graphe* getNodeWithVirtualAddr(Graphe* g, uintptr_t va) {
    if (g->_immediat == PASSAGE_GET_NODE_WITH_VIRTUALADDR) {
        return NULL;
    }
    uint8_t etatinit = g->_immediat;
    g->_immediat = PASSAGE_GET_NODE_WITH_VIRTUALADDR;
    if (g->VirtualAddr == va) {
        g->_immediat = etatinit;
        return g;
    }
    if (g->listeFils == NULL) {
        g->_immediat = etatinit;
        return NULL;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        Graphe* res = getNodeWithVirtualAddr(getFirstLL(tete), va);
        if (res != NULL) {
            g->_immediat = etatinit;
            return res;
        }
        tete = tete->suiv;
    }
    g->_immediat = etatinit;
    return NULL;
}

Graphe* getNodeWithVirtualAddrUnique(Graphe* g, uintptr_t va) {
    if (g->_immediat == PASSAGE_GET_NODE_WITH_VIRTUALADDR_U) {
        return NULL;
    }
    g->_immediat = PASSAGE_GET_NODE_WITH_VIRTUALADDR_U;
    if (g->VirtualAddr == va) {
        return g;
    }
    if (g->listeFils == NULL) {
        return NULL;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        Graphe* res = getNodeWithVirtualAddr(getFirstLL(tete), va);
        if (res != NULL) {
            return res;
        }
        tete = tete->suiv;
    }
    return NULL;
}

/*------------------------ FONCTIONS DE TRAITEMENT --------------------------*/

static char* parseRegistre(ARGTYPE arg){
    char* res = malloc(sizeof(char)*32);
    int tailleArg = strlen(arg.ArgMnemonic);
    for (int i = 0; i<tailleArg; i++) {
        if (arg.ArgMnemonic[i] == '+' || arg.ArgMnemonic[i] == '-') {
            return res;
        }
        res[i] = arg.ArgMnemonic[i];
    }
    return res;
}

static Registre * getGeneralRegistre(ARGTYPE arg, Processeur *proc) { 
    // TODO à refaire
    char* mnemo = parseRegistre(arg);
//    uint16_t loword = arg.ArgType;
//    int size = arg.ArgSize;
//    Registre* res = NULL;
    
    if (strcmp(mnemo, "rax") == 0) {
        return _RAX;
    }
    if (strcmp(mnemo, "eax") == 0) {
        return _EAX;
    }
    if (strcmp(mnemo, "ax") == 0) {
        return _AX;
    }
    if (strcmp(mnemo, "ah") == 0) {
        return _AH;
    }
    if (strcmp(mnemo, "al") == 0) {
        return _AL;
    }
    
    if (strcmp(mnemo, "rbx") == 0) {
        return _RBX;
    }
    if (strcmp(mnemo, "ebx") == 0) {
        return _EBX;
    }
    if (strcmp(mnemo, "bx") == 0) {
        return _BX;
    }
    if (strcmp(mnemo, "bh") == 0) {
        return _BH;
    }
    if (strcmp(mnemo, "bl") == 0) {
        return _BL;
    }
    
    if (strcmp(mnemo, "rcx") == 0) {
        return _RCX;
    }
    if (strcmp(mnemo, "ecx") == 0) {
        return _ECX;
    }
    if (strcmp(mnemo, "cx") == 0) {
        return _CX;
    }
    if (strcmp(mnemo, "ch") == 0) {
        return _CH;
    }
    if (strcmp(mnemo, "cl") == 0) {
        return _CL;
    }
    
    if (strcmp(mnemo, "rdx") == 0) {
        return _RDX;
    }
    if (strcmp(mnemo, "edx") == 0) {
        return _EDX;
    }
    if (strcmp(mnemo, "dx") == 0) {
        return _DX;
    }
    if (strcmp(mnemo, "dh") == 0) {
        return _DH;
    }
    if (strcmp(mnemo, "dl") == 0) {
        return _DL;
    }
    
    if (strcmp(mnemo, "rsp") == 0) {
        return _RSP;
    }
    if (strcmp(mnemo, "esp") == 0) {
        return _ESP;
    }
    if (strcmp(mnemo, "sp") == 0) {
        return _SP;
    }
    if (strcmp(mnemo, "spl") == 0) {
        return _SPL;
    }
    
    if (strcmp(mnemo, "rbp") == 0) {
        return _RBP;
    }
    if (strcmp(mnemo, "ebp") == 0) {
        return _EBP;
    }
    if (strcmp(mnemo, "bp") == 0) {
        return _BP;
    }
    if (strcmp(mnemo, "bpl") == 0) {
        return _BPL;
    }
    
    if (strcmp(mnemo, "rsi") == 0) {
        return _RSI;
    }
    if (strcmp(mnemo, "esi") == 0) {
        return _ESI;
    }
    if (strcmp(mnemo, "si") == 0) {
        return _SI;
    }
    if (strcmp(mnemo, "sil") == 0) {
        return _SIL;
    }
    
    if (strcmp(mnemo, "rdi") == 0) {
        return _RDI;
    }
    if (strcmp(mnemo, "edi") == 0) {
        return _EDI;
    }
    if (strcmp(mnemo, "di") == 0) {
        return _DI;
    }
    if (strcmp(mnemo, "dil") == 0) {
        return _DIL;
    }
    
    if (strcmp(mnemo, "r8") == 0) {
        return _R8;
    }
    if (strcmp(mnemo, "r8d") == 0) {
        return _R8D;
    }
    if (strcmp(mnemo, "r8w") == 0) {
        return _R8W;
    }
    if (strcmp(mnemo, "r8L") == 0) {
        return _R8B;
    }
    
    if (strcmp(mnemo, "r9") == 0) {
        return _R9;
    }
    if (strcmp(mnemo, "r9d") == 0) {
        return _R9D;
    }
    if (strcmp(mnemo, "r9w") == 0) {
        return _R9W;
    }
    if (strcmp(mnemo, "r9L") == 0) {
        return _R9B;
    }
    
    if (strcmp(mnemo, "r10") == 0) {
        return _R10;
    }
    if (strcmp(mnemo, "r10d") == 0) {
        return _R10D;
    }
    if (strcmp(mnemo, "r10w") == 0) {
        return _R10W;
    }
    if (strcmp(mnemo, "r10L") == 0) {
        return _R10B;
    }
    
    if (strcmp(mnemo, "r11") == 0) {
        return _R11;
    }
    if (strcmp(mnemo, "r11d") == 0) {
        return _R11D;
    }
    if (strcmp(mnemo, "r11w") == 0) {
        return _R11W;
    }
    if (strcmp(mnemo, "r11L") == 0) {
        return _R11B;
    }
    
    if (strcmp(mnemo, "r12") == 0) {
        return _R12;
    }
    if (strcmp(mnemo, "r12d") == 0) {
        return _R12D;
    }
    if (strcmp(mnemo, "r12w") == 0) {
        return _R12W;
    }
    if (strcmp(mnemo, "r12L") == 0) {
        return _R12B;
    }
    
    if (strcmp(mnemo, "r13") == 0) {
        return _R13;
    }
    if (strcmp(mnemo, "r13d") == 0) {
        return _R13D;
    }
    if (strcmp(mnemo, "r13w") == 0) {
        return _R13W;
    }
    if (strcmp(mnemo, "r13L") == 0) {
        return _R13B;
    }
    
    if (strcmp(mnemo, "r14") == 0) {
        return _R14;
    }
    if (strcmp(mnemo, "r14d") == 0) {
        return _R14D;
    }
    if (strcmp(mnemo, "r14w") == 0) {
        return _R14W;
    }
    if (strcmp(mnemo, "r14L") == 0) {
        return _R14B;
    }
    
    if (strcmp(mnemo, "r15") == 0) {
        return _R15;
    }
    if (strcmp(mnemo, "r15d") == 0) {
        return _R15D;
    }
    if (strcmp(mnemo, "r15w") == 0) {
        return _R15W;
    }
    if (strcmp(mnemo, "r15L") == 0) {
        return _R15B;
    }
    
    printf("Erreur : registre général inconnu");
    exit(EXIT_FAILURE);
//    switch (loword) {
//        case REG0:
//            switch (size) {
//                case 64:
//                    res = _RAX;
//                    break;
//                case 32:
//                    res = _EAX;
//                    break;
//                case 16:
//                    res = _AX;
//                    break;
//                case 8:
//                    if (arg.ArgPosition == HighPosition) {
//                        res = _AH;
//                    } else {
//                        res = _AL;
//                    }
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG1:
//            switch (size) {
//                case 64:
//                    res = _RCX;
//                    break;
//                case 32:
//                    res = _ECX;
//                    break;
//                case 16:
//                    res = _CX;
//                    break;
//                case 8:
//                    if (arg.ArgPosition == HighPosition) {
//                        res = _CH;
//                    } else {
//                        res = _CL;
//                    }
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG2:
//            switch (size) {
//                case 64:
//                    res = _RDX;
//                    break;
//                case 32:
//                    res = _EDX;
//                    break;
//                case 16:
//                    res = _DX;
//                    break;
//                case 8:
//                    if (arg.ArgPosition == HighPosition) {
//                        res = _DH;
//                    } else {
//                        res = _DL;
//                    }
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG3:
//            switch (size) {
//                case 64:
//                    res = _RBX;
//                    break;
//                case 32:
//                    res = _EBX;
//                    break;
//                case 16:
//                    res = _BX;
//                    break;
//                case 8:
//                    if (arg.ArgPosition == HighPosition) {
//                        res = _BH;
//                    } else {
//                        res = _BL;
//                    }
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG4:
//            switch (size) {
//                case 64:
//                    res = _RSP;
//                    break;
//                case 32:
//                    res = _ESP;
//                    break;
//                case 16:
//                    res = _SP;
//                    break;
//                case 8:
//                    res = _SPL;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG5:
//            switch (size) {
//                case 64:
//                    res = _RBP;
//                    break;
//                case 32:
//                    res = _EBP;
//                    break;
//                case 16:
//                    res = _BP;
//                    break;
//                case 8:
//                    res = _BPL;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG6:
//            switch (size) {
//                case 64:
//                    res = _RSI;
//                    break;
//                case 32:
//                    res = _ESI;
//                    break;
//                case 16:
//                    res = _SI;
//                    break;
//                case 8:
//                    res = _SIL;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG7:
//            switch (size) {
//                case 64:
//                    res = _RDI;
//                    break;
//                case 32:
//                    res = _EDI;
//                    break;
//                case 16:
//                    res = _DI;
//                    break;
//                case 8:
//                    res = _DIL;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG8:
//            switch (size) {
//                case 64:
//                    res = _R8;
//                    break;
//                case 32:
//                    res = _R8D;
//                    break;
//                case 16:
//                    res = _R8W;
//                    break;
//                case 8:
//                    res = _R8B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG9:
//            switch (size) {
//                case 64:
//                    res = _R9;
//                    break;
//                case 32:
//                    res = _R9D;
//                    break;
//                case 16:
//                    res = _R9W;
//                    break;
//                case 8:
//                    res = _R9B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG10:
//            switch (size) {
//                case 64:
//                    res = _R10;
//                    break;
//                case 32:
//                    res = _R10D;
//                    break;
//                case 16:
//                    res = _R10W;
//                    break;
//                case 8:
//                    res = _R10B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG11:
//            switch (size) {
//                case 64:
//                    res = _R11;
//                    break;
//                case 32:
//                    res = _R11D;
//                    break;
//                case 16:
//                    res = _R11W;
//                    break;
//                case 8:
//                    res = _R11B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG12:
//            switch (size) {
//                case 64:
//                    res = _R12;
//                    break;
//                case 32:
//                    res = _R12D;
//                    break;
//                case 16:
//                    res = _R12W;
//                    break;
//                case 8:
//                    res = _R12B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG13:
//            switch (size) {
//                case 64:
//                    res = _R13;
//                    break;
//                case 32:
//                    res = _R13D;
//                    break;
//                case 16:
//                    res = _R13W;
//                    break;
//                case 8:
//                    res = _R13B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG14:
//            switch (size) {
//                case 64:
//                    res = _R14;
//                    break;
//                case 32:
//                    res = _R14D;
//                    break;
//                case 16:
//                    res = _R14W;
//                    break;
//                case 8:
//                    res = _R14B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        case REG15:
//            switch (size) {
//                case 64:
//                    res = _R15;
//                    break;
//                case 32:
//                    res = _R15D;
//                    break;
//                case 16:
//                    res = _R15W;
//                    break;
//                case 8:
//                    res = _R15B;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        default:
//            break;
//    }
//    return res;
}

/*!
 * @deprecated
 */
static void setGeneralRegistre(int i, ARGTYPE argument[], Processeur*proc, Registre**reg) {
    reg[i] = getGeneralRegistre(argument[i], proc);
}

Registre * getRegistre(ARGTYPE arg, Processeur *newPool) {
    Registre *res;
    uint32_t hi = arg.ArgType & 0xffff0000;
    switch (hi) {
        case REGISTER_TYPE + MMX_REG:
            ;
            break;
        case REGISTER_TYPE + GENERAL_REG:
            res = getGeneralRegistre(arg, newPool);
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
        default:
            printf("l'argument n'est pas un registre\n");
            exit(EXIT_FAILURE);
            break;
    }
    return res;
}

Registre * getConstant(ARGTYPE arg, DISASM *disasm) {
    uint32_t hi = arg.ArgType & 0xffff0000;
    Registre* r = NULL;
    switch (hi) {
        case CONSTANT_TYPE + RELATIVE_: //faux
            r = newRegistreFeuille(64);
            setRegVal(r, disasm->Instruction.Immediat);
            break;
        case CONSTANT_TYPE + ABSOLUTE_:
            r = newRegistreFeuille(64);
            setRegVal(r, disasm->Instruction.Immediat);
            break;
        default:
            printf("l'argument n'est pas une constante\n");
            exit(EXIT_FAILURE);
            break;
    }
    return r;
}

Segment getMemory(ARGTYPE arg, /*DISASM* disasm,*/ Processeur* newPool){
    char* mnemo = arg.ArgMnemonic; 
    char prem = *mnemo;
    if (48 <= prem && prem <= 57) {
        return seg(newPool->mem, arg.Memory.Displacement, arg.ArgSize/8);
    }
    if (arg.Memory.Displacement == 0) {
        Registre* reg = getGeneralRegistre(arg, newPool);
        return seg(newPool->mem, getRegVal(reg), arg.ArgSize/8);
    } else {
        Registre* reg = getGeneralRegistre(arg, newPool);
        return seg(newPool->mem, getRegVal(reg) + arg.Memory.Displacement, arg.ArgSize/8);
    }
}

/*!
 * Cette fonction lit l'instruction du noeud g à partir du (nouveau) pool de 
 * son père
 * Ici g ne sert qu'à donner l'instruction (on utilise pas son pool)
 */

static void setPool(const Graphe* g, Processeur* newPool) {
    DISASM* disasm = newDisasm();
    disasm->Archi = g->pool->archi;
    disasm->EIP = g->aif;
    disasm->VirtualAddr = g->VirtualAddr;
    int len = Disasm(disasm);

    INSTRTYPE instr = disasm->Instruction;

    Instruction* instruction = NULL;
    Variable var[3];

    /* initialisation de l'instruction */
    
    if (strcmp(instr.Mnemonic, "add ") == 0) {
        instruction = init_add();
    } else if (strcmp(instr.Mnemonic, "mov ") == 0) {
        instruction = init_mov();
    } else if (strcmp(instr.Mnemonic, "shl ") == 0) {
        instruction = init_shl();
    } else if (strcmp(instr.Mnemonic, "shr ") == 0) {
        instruction = init_shr();
    } else if (strcmp(instr.Mnemonic, "cmp ") == 0) {
        instruction = init_cmp();
    } else if (strcmp(instr.Mnemonic, "je ") == 0) {
        instruction = init_je();
    } else if (strcmp(instr.Mnemonic, "imul ") == 0) {
        instruction = init_imul();
    } else if (strcmp(instr.Mnemonic, "sub ") == 0) {
        instruction = init_sub();
    } else if (strcmp(instr.Mnemonic, "inc ") == 0) {
        instruction = init_inc();
    }
    
//    switch (instr.Opcode) {
//        case 0x00:
//            //instruction = init_add_EbGb();
//            instruction = init_add();
//            break;
//        case 0x01:
//            //instruction = init_add_EbGb();
//            instruction = init_add();
//            break;
//
//        default:
//            //            printf("litInstruction() : opcode inconnu\n");
//            //            exit(EXIT_FAILURE);
//            break;
//    }

    ARGTYPE argument[] = {
        disasm->Argument1,
        disasm->Argument2,
        disasm->Argument3
    };

    uint8_t aSuppr[] = {0, 0, 0};

    /* initialisation des arguments*/
    for (int i = 0; i < 3; i++) {
        ARGTYPE arg = argument[i];

        uint32_t hiword = arg.ArgType & 0xffff0000;
        Variable res;
        memset(&res, 0, sizeof(Variable));

        switch (hiword & 0xf0000000) {
            case NO_ARGUMENT:
                ;
                break;
            case REGISTER_TYPE:
                res.type = reg_type;
                res.reg = getRegistre(arg, newPool);
                break;

            case CONSTANT_TYPE:
                res.type = reg_type;
                res.reg = getConstant(arg, disasm);
                aSuppr[i] = 1;
                break;

            case MEMORY_TYPE:
                if (strcmp(instr.Mnemonic, "push ") == 0) break;
                if (strcmp(instr.Mnemonic, "pop ") == 0 && i>0) break;
                res.type = seg_type;
                res.reg = NULL;
                res.seg = getMemory(arg, newPool);
                break;

            default:
                break;
        }

        var[i] = res;
    }
    if (instruction != NULL) {
        do_instr(instruction, var[0], var[1], var[2], len, newPool);
        terminateInstruction(instruction);
    }
    for (int i = 0; i < 3; i++) {
        if (aSuppr[i]) {
            terminateRegistre(var[i].reg);
        }
    }

    free(disasm);
}

/*!
 * Cette version de l'algorithme de propagation des constantes est une 
 * optimisation mémoire de l'algo de Kildall. Il se fait en espace constant.
 * 
 * initialPool n'est pas vraiment constant car on peut eventuellement y acceder 
 * à partir de g qui lui n'est pas constant.
 */

static void optimizePool_aux(Graphe* g, const Processeur* initialPool, Fichier* fichierlog, char temp[MAX_BUFFER]) {
    sprintf(temp, "optimise 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);

    Processeur* copyPool = newProcesseurCopy(initialPool);
    setPool(g, copyPool);
    int inc = incluDans(g->pool, copyPool, fichierlog);
    if (inc != NON_INCLUS) {
        terminateProcesseur(copyPool);
        sprintf(temp, "fin de 0x%llx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, copyPool); // l'intercection est dans g->pool
    terminateProcesseur(copyPool);
    if (g->listeFils == NULL) {
        sprintf(temp, "fin de 0x%llx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        optimizePool_aux(tete->valeur, g->pool, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp, "fin de 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/*!
 * Cette version de l'algorithme de propagation des constantes est une 
 * optimisation mémoire de l'algo de Kildall. Il se fait en espace constant.
 *
 * initialPool n'est pas vraiment constant car on peut eventuellement y acceder 
 * à partir de g qui lui n'est pas constant.
 */

static void optimizePool_aux2(Graphe* g, const Graphe* pere, Fichier* fichierlog, char temp[MAX_BUFFER]) {
    sprintf(temp, "optimise 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);

    g->pool->archi = pere->pool->archi;
    Processeur* copyPool = newProcesseurCopy(pere->pool);
    setPool(pere, copyPool);
    int inc = incluDans(g->pool, copyPool, fichierlog);
    if (inc != NON_INCLUS) {
        terminateProcesseur(copyPool);
        sprintf(temp, "fin de 0x%llx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, copyPool); // l'intercection est dans g->pool
    terminateProcesseur(copyPool);
    if (g->listeFils == NULL) {
        sprintf(temp, "fin de 0x%llx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        optimizePool_aux2(tete->valeur, g, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp, "fin de 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/*!
 * Cette version de propagation des constante respecte l'agorithme de Kildall
 * (a utiliser dans optimizePool (version 1)
 */

static void optimizePool_aux_kildall(Graphe* g, const Processeur* initialPool, Fichier* fichierlog, char temp[MAX_BUFFER]) {
    sprintf(temp, "optimise 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);

    int inc = incluDans(g->pool, initialPool, fichierlog);
    if (inc != NON_INCLUS) {
        sprintf(temp, "fin de 0x%llx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    inter(g->pool, initialPool); // l'intercection est dans g->pool
    if (g->listeFils == NULL) {
        sprintf(temp, "fin de 0x%llx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    Processeur* newPool = newProcesseurCopy(g->pool);
    setPool(g, newPool); // ici newPool est modifie par g
    int l = sizeLL(g->listeFils);
    pushlog(fichierlog, temp);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        optimizePool_aux_kildall(tete->valeur, newPool, fichierlog, temp);
        tete = tete->suiv;
    }
    terminateProcesseur(newPool);
    sprintf(temp, "fin de 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);
}

/*
 * Il ne faut pas que initialPool soit modifié car sinon on modifierait le 
 * pool du père
 */

void optimizePool(Graphe* g, const Processeur* initialPool) {
    char chemin_log[FILENAME_MAX];
    strcpy(chemin_log, ROOT);
    strcat(chemin_log, CHEMIN_LOG_OPTIMISATION);
    Fichier* fichierlog = newFichier(chemin_log);
    char temp[MAX_BUFFER];

    optimizePool_aux(g, initialPool, fichierlog, temp);

    terminateFichier(fichierlog);
}

void optimizePool2(Graphe* g, const Processeur* initialPool) {
    Fichier* fichierlog = newFichier(CHEMIN_LOG_OPTIMISATION);
    char temp[MAX_BUFFER];

    sprintf(temp, "optimise 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);

    g->pool->archi = initialPool->archi;
    int inc = incluDans(g->pool, initialPool, fichierlog);
    if (inc != NON_INCLUS) {
        sprintf(temp, "fin de 0x%llx par inclusion\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }

    inter(g->pool, initialPool); // l'intercection est dans g->pool

    if (g->listeFils == NULL) {
        sprintf(temp, "fin de 0x%llx par manque de fils\n", g->VirtualAddr);
        pushlog(fichierlog, temp);
        return;
    }
    int l = sizeLL(g->listeFils);
    LinkedList* tete = g->listeFils;
    for (int i = 0; i < l; i++) {
        Graphe* fils = tete->valeur;
        optimizePool_aux2(fils, g, fichierlog, temp);
        tete = tete->suiv;
    }
    sprintf(temp, "fin de 0x%llx\n", g->VirtualAddr);
    pushlog(fichierlog, temp);

    terminateFichier(fichierlog);
}

DISASM* newDisasmFromGraph(Graphe* g) {
    DISASM* disasm = newDisasm();
    disasm->EIP = g->aif;
    disasm->VirtualAddr = g->VirtualAddr;
    disasm->Archi = g->pool->archi;
    Disasm(disasm);
    return disasm;
}

int debranchage_fils_aux(Graphe*, int8_t);

/*
 * Cette fonction est reccursive car elle rappelle debranchage
 */

static int debranche_fils_direct(Graphe* g, int8_t marqueur) {
    if (g->listeFils == NULL || sizeLL(g->listeFils) == 0) {
        return 0;
    }
    int res = 0;
    Graphe* G = (Graphe*) getFirstLL(g->listeFils);
    if (G->VirtualAddr == (g->VirtualAddr + g->tailleInstruction)) {
        removeLinkRec(g, G);
        res = 1;
    } else if (sizeLL(g->listeFils) == 2) {
        removeLinkRec(g, getLastLL(g->listeFils));
        res = 1;
    }
    int l = sizeLL(g->listeFils);
    if (l>1) {
        printf("La configuration où un saut conditionnel a strictement plus de deux fils est impossible.\n");
        exit(EXIT_FAILURE);
    }
    Graphe* fils = getFirstLL(g->listeFils);
    int res2 = debranchage_fils_aux(fils, marqueur);
    return res | res2;
}

/*
 * Cette fonction est reccursive car elle rappelle debranchage
 */

static int debranche_fils_saut(Graphe* g, int8_t marqueur) {
    if (g->listeFils == NULL || sizeLL(g->listeFils) == 0) {
        return 0;
    }
    int res = 0;
    Graphe* G = (Graphe*) getFirstLL(g->listeFils);
    if (G->VirtualAddr != (g->VirtualAddr + g->tailleInstruction)) {
        removeLinkRec(g, G);
        res = 1;
    } else if (sizeLL(g->listeFils) == 2) {
        removeLinkRec(g, getLastLL(g->listeFils));
        res = 1;
    }
    int l = sizeLL(g->listeFils);
    if (l>1) {
        printf("La configuration où un saut conditionnel a strictement plus de deux fils est impossible.\n");
        exit(EXIT_FAILURE);
    }
    Graphe* fils = getFirstLL(g->listeFils);
    int res2 = debranchage_fils_aux(fils, marqueur);
    return res | res2;
}

/*
 * Cette fonction est recursive dans les appelles des fonctions de debranchage 
 * des fils.
 */

int debranchage_fils_aux(Graphe* g, int8_t marqueur) {
    if (g==NULL) {
        return 0;
    }
    if (g->_immediat == marqueur) {
        return 0;
    }
    g->_immediat = marqueur;
    DISASM* disasm = newDisasmFromGraph(g);

    int branch = disasm->Instruction.BranchType;
    int res = 0;
    int8_t* tabFlags = g->pool->tabFlags;
    switch (branch) {
        case JO:
            if (tabFlags[_nOF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nOF] == FLAG_BAS) {
                res = debranche_fils_saut(g, marqueur);
            }
            break;

        case JNO:
            if (tabFlags[_nOF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nOF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }
            break;

        case JC:
            if (g->pool->tabFlags[_nCF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (g->pool->tabFlags[_nCF] == FLAG_BAS){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JNC:
            if (tabFlags[_nCF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nCF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }
            break;

        case JE:
            if (tabFlags[_nZF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_BAS){
                res = debranche_fils_saut(g, marqueur);
            }
            break;

        case JNE:
            if (tabFlags[_nZF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JA:
            if (tabFlags[_nZF] == FLAG_BAS && tabFlags[_nCF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_HAUT || tabFlags[_nCF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JNA:
            if (tabFlags[_nZF] == FLAG_HAUT || tabFlags[_nCF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_BAS && tabFlags[_nCF] == FLAG_BAS){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;



        case JS:
            if (tabFlags[_nSF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nSF] == FLAG_BAS){
                res = debranche_fils_saut(g, marqueur);
            }
            ;
            break;

        case JNS:
            if (tabFlags[_nSF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nSF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JP:
            if (tabFlags[_nPF] == FLAG_HAUT) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nPF] == FLAG_BAS) {
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JNP:
            if (tabFlags[_nPF] == FLAG_BAS) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nPF] == FLAG_HAUT){
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JL:
            if ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_HAUT) ||
                    (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_BAS)) {
                res = debranche_fils_direct(g, marqueur);
            } else if ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_BAS) ||
                       (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_HAUT)){
                res = debranche_fils_saut(g, marqueur);
            }
            ;
            break;

        case JNL:
            if ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_BAS) ||
                    (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_HAUT)) {
                res = debranche_fils_direct(g, marqueur);
            } else if ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_HAUT) ||
                       (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_BAS)){
                res = debranche_fils_saut(g, marqueur);
            }
            ;
            break;

        case JG:
            if (tabFlags[_nZF] == FLAG_BAS &&
                    ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_BAS) ||
                    (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_HAUT))) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_HAUT ||
                    (tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_HAUT) ||
                    (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_BAS)) {
                res = debranche_fils_saut(g, marqueur);
            }
            ;
            break;

        case JNG:
            if (tabFlags[_nZF] == FLAG_HAUT ||
                    (tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_HAUT) ||
                    (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_BAS)) {
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nZF] == FLAG_BAS &&
                       ((tabFlags[_nSF] == FLAG_BAS && tabFlags[_nOF] == FLAG_BAS) ||
                        (tabFlags[_nSF] == FLAG_HAUT && tabFlags[_nOF] == FLAG_HAUT))) {
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JB:
            if (tabFlags[_nCF] == FLAG_HAUT) {//si CF à 1 saut
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nCF] == FLAG_BAS) {
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;


        case JNB:
            if (tabFlags[_nCF] == FLAG_BAS) {//si CF à 0 saut
                res = debranche_fils_direct(g, marqueur);
            } else if (tabFlags[_nCF] == FLAG_HAUT) {
                res = debranche_fils_saut(g, marqueur);
            }

            ;
            break;

        case JECXZ:
            if (getRegVal(g->pool->tabRegistre[_nECX]) == 0) {//si ECX=0 on saute
                res = debranche_fils_direct(g, marqueur);
            } else {
                res = debranche_fils_saut(g, marqueur);

            }
            break;

        default:
            if (g->listeFils == NULL) {
                return 0;
            }
            int l = sizeLL(g->listeFils);
            LinkedList* tete = g->listeFils;
            int res2 = 0;
            for (int i = 0; i<l; i++) {
                Graphe* fils = tete->valeur;
                res2 = debranchage_fils_aux(fils, marqueur) | res2;
            }
            return res2;
            break;
    }
    free(disasm);
    return res;
}

int debranchage_fils(Graphe* g){
    srand (time(NULL));
    int8_t marqueur = rand();
    while (marqueur == g->_immediat) {
        marqueur = rand();
    }
    return debranchage_fils_aux(g, marqueur);
}

void elagage(Graphe* g, Processeur* poolInit){
    reinitPools(g);
    optimizePool2(g, poolInit);
    int res = debranchage_fils(g);
    while (res == 1) {
        reinitPools(g);
        optimizePool2(g, poolInit);
        res = debranchage_fils(g);
    }
}



static void enregistrePropagation_aux(Fichier*file, Graphe*g, int8_t marqueur){
    if (g->_immediat == marqueur) {
        return;
    }
    char temp[1024];
    g->_immediat = marqueur;
    sprintf(temp, "\n=====================================================\n**********         Noeud 0x%llx         **********\n=====================================================\n\n", g->VirtualAddr);
    pushlog(file, temp);
    enregistrerPool(file, g->pool);
    LinkedList* listeFils = g->listeFils;
    if (listeFils == NULL) {
        return;
    }
    LinkedList* tete = listeFils;
    for (int i = 0; i<sizeLL(listeFils); i++) {
        enregistrePropagation_aux(file, tete->valeur, marqueur);
        tete = tete->suiv;
    }
}

void enregistrePropagation(Fichier* file , Graphe* g){
    cleanFile(file);
    srand (time(NULL));
    int8_t marqueur = rand();
    while (marqueur == g->_immediat) {
        marqueur = rand();
    }
    enregistrePropagation_aux(file, g, marqueur);
}















