//
//  processeur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "processeur.h"

#define RAX proc->rax
#define EAX proc->eax
#define AX  proc->ax
#define AH  proc->ah
#define AL  proc->al

#define RBX proc->rbx
#define EBX proc->ebx
#define BX  proc->bx
#define BH  proc->bh
#define BL  proc->bl

#define RCX proc->rcx
#define ECX proc->ecx
#define CX  proc->cx
#define CH  proc->ch
#define CL  proc->cl

#define RDX proc->rdx
#define EDX proc->edx
#define DX  proc->dx
#define DH  proc->dh
#define DL  proc->dl

#define RDI proc->rdi
#define EDI proc->edi

#define RSI proc->rsi
#define ESI proc->esi

#define RBP proc->rbp
#define EBP proc->ebp

#define RSP proc->rsp
#define ESP proc->esp

#define RIP proc->rip
#define EIP proc->eip

#define RFLAGS proc->rflags
#define EFLAGS proc->eflags

#define R8  proc->r8
#define R9  proc->r9
#define R10  proc->r10
#define R11  proc->r11
#define R12  proc->r12
#define R13  proc->r13
#define R14  proc->r14
#define R15  proc->r15

#define CS  proc->cs
#define DS  proc->ds
#define SS  proc->ss
#define ES  proc->es
#define FS  proc->fs
#define GS  proc->gs

#define CF proc->CF
#define PF proc->PF
#define AF proc->AF
#define ZF proc->ZF
#define SF proc->SF
#define TF proc->TF
#define IF proc->IF
#define DF proc->DF
#define OF proc->OF

#define STACK proc->stack

Processeur* newProcesseur(){
    Processeur* proc = malloc(sizeof(Processeur));
    AL = newRegistreFeuille(8, 0);
    AH = newRegistreFeuille(8, 0);
    AX = newRegistreDeuxFils(AH, AL);
    EAX = newRegistreFilsUnique(AX);
    RAX = newRegistreFilsUnique(EAX);
    
    BL = newRegistreFeuille(8, 0);
    BH = newRegistreFeuille(8, 0);
    BX = newRegistreDeuxFils(BH, BL);
    EBX = newRegistreFilsUnique(BX);
    RBX = newRegistreFilsUnique(EBX);
    
    CL = newRegistreFeuille(8, 0);
    CH = newRegistreFeuille(8, 0);
    CX = newRegistreDeuxFils(CH, CL);
    ECX = newRegistreFilsUnique(CX);
    RCX = newRegistreFilsUnique(ECX);
    
    DL = newRegistreFeuille(8, 0);
    DH = newRegistreFeuille(8, 0);
    DX = newRegistreDeuxFils(DH, DL);
    EDX = newRegistreFilsUnique(DX);
    RDX = newRegistreFilsUnique(EDX);
    
    EDI = newRegistreFeuille(32, 0);
    RDI = newRegistreFilsUnique(EDI);
    
    ESI = newRegistreFeuille(32, 0);
    RSI = newRegistreFilsUnique(ESI);
    
    EBP = newRegistreFeuille(32, 0);
    RBP = newRegistreFilsUnique(EBP);
    
    ESP = newRegistreFeuille(32, 0);
    RSP = newRegistreFilsUnique(ESP);
    
    EIP = newRegistreFeuille(32, 0);
    RIP = newRegistreFilsUnique(EIP);
    
    EFLAGS = newRegistreFeuille(32, 0);
    RFLAGS = newRegistreFilsUnique(EFLAGS);
    
    R8 = newRegistreFeuille(64, 0);
    R9 = newRegistreFeuille(64, 0);
    R10 = newRegistreFeuille(64, 0);
    R11 = newRegistreFeuille(64, 0);
    R12 = newRegistreFeuille(64, 0);
    R13 = newRegistreFeuille(64, 0);
    R14 = newRegistreFeuille(64, 0);
    R15 = newRegistreFeuille(64, 0);
    
    STACK = newLinkedList();
    
    return proc;
}

void terminateProcesseur(Processeur* proc){
    terminateRegistre(RAX);
    terminateRegistre(RBX);
    terminateRegistre(RCX);
}

void _call(Processeur* proc, int len, Registre* adresse){
    addFirstLL(STACK, (void *) (RIP + len));
    copieVal(RIP, adresse);
}

void _jmp(Processeur* proc, Registre* adresse){
    copieVal(RIP, adresse);
}

void _jne(Processeur* proc, int len, Registre* adresse){
    if (!ZF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _je(Processeur* proc, int len, Registre* adresse){
    if (ZF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _ja(Processeur* proc, int len, Registre* adresse){
    if (!ZF && !CF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _jb(Processeur* proc, int len, Registre* adresse){
    if (CF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _jbe(Processeur* proc, int len, Registre* adresse){
    if (ZF || CF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _jg(Processeur* proc, int len, Registre* adresse){
    if (!ZF && SF == OF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _jle(Processeur* proc, int len, Registre* adresse){
    if (ZF || SF != OF) {
        _jmp(proc, adresse);
    } else {
        RIP += len;
    }
}

void _and(Processeur* proc, Registre* destination, Registre* masque){
    uint64_t dest = getValeur(destination);
    uint64_t mask = getValeur(masque);
    dest &= mask;
    setValeur(destination, dest);
}

void _move(Registre* dest, Registre* source){
    *dest = *source;
}









