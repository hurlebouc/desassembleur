//
//  desassembleur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//


#include "desassembleur.h"
#include "_macro_Build.h"


desasembleur* newDesassembleur(desasembleur* desas){
    
    if (desas == NULL) {
        desas = malloc(sizeof(desasembleur));
        desas->disasm = malloc(sizeof(DISASM));;
        desas->proc = newProcesseur();
    }
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(desas->disasm, 0, sizeof (DISASM));
    
    return desas;
}

void terminateDesassembleur(desasembleur* desas){
    free(desas->disasm);
    terminateLinkedList(desas->proc->stack); // on utilisera pas de lien vers l'extérieur
    terminateProcesseur(desas->proc);
    free(desas);
}

static void setRegistre(int i, ARGTYPE* argument, Processeur *proc, Registre** reg) {
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
                default:
                    break;
            }
            break;
        case REG8:
            reg[i] = _R8;
        case REG9:
            reg[i] = _R9;
        case REG10:
            reg[i] = _R10;
        case REG11:
            reg[i] = _R11;
        case REG12:
            reg[i] = _R12;
        case REG13:
            reg[i] = _R13;
        case REG14:
            reg[i] = _R14;
        case REG15:
            reg[i] = _R15;
        default:
            break;
    }
}

int litInstruction(desasembleur* desas){
    int len = Disasm(desas->disasm);
    Processeur* proc = desas->proc;
    INSTRTYPE instr = desas->disasm->Instruction;
    
    Instruction* instruction = NULL;
    Registre* reg[] = {NULL, NULL, NULL};
    
    switch (instr.Opcode) {
        case 0x00:
//            instruction = init_add_EbGb();
            instruction = init_add();
            break;
            
        default:
            printf("litInstruction() : opcode inconnu\n");
            exit(EXIT_FAILURE);
            break;
    }
    
    ARGTYPE argument[] = {
        desas->disasm->Argument1,
        desas->disasm->Argument2,
        desas->disasm->Argument3
    };
    
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
                setRegistre(i, argument, proc, reg);
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
                ;
                break;
                
            default:
                break;
        }
    }
    
    
    do_instr(instruction, reg[0], reg[1], len, proc);
    return len;
}