//
//  processeur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_processeur_h
#define desassembleur_processeur_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include "LinkedList.h"
#include "registre.h"
    
#define _RAX proc->rax
#define _EAX proc->eax
#define _AX  proc->ax
#define _AH  proc->ah
#define _AL  proc->al
    
#define _RBX proc->rbx
#define _EBX proc->ebx
#define _BX  proc->bx
#define _BH  proc->bh
#define _BL  proc->bl
    
#define _RCX proc->rcx
#define _ECX proc->ecx
#define _CX  proc->cx
#define _CH  proc->ch
#define _CL  proc->cl
    
#define _RDX proc->rdx
#define _EDX proc->edx
#define _DX  proc->dx
#define _DH  proc->dh
#define _DL  proc->dl
    
#define _RDI proc->rdi
#define _EDI proc->edi
    
#define _RSI proc->rsi
#define _ESI proc->esi
    
#define _RBP proc->rbp
#define _EBP proc->ebp
    
#define _RSP proc->rsp
#define _ESP proc->esp
    
#define _RIP proc->rip
#define _EIP proc->eip
    
#define _RFLAGS proc->rflags
#define _EFLAGS proc->eflags
    
#define _R8  proc->r8
#define _R9  proc->r9
#define _R10  proc->r10
#define _R11  proc->r11
#define _R12  proc->r12
#define _R13  proc->r13
#define _R14  proc->r14
#define _R15  proc->r15
    
#define _CS  proc->cs
#define _DS  proc->ds
#define _SS  proc->ss
#define _ES  proc->es
#define _FS  proc->fs
#define _GS  proc->gs
    
#define _CF proc->_cf
#define _PF proc->_pf
#define _AF proc->_af
#define _ZF proc->_zf
#define _SF proc->_sf
#define _TF proc->_tf
#define _IF proc->_if
#define _DF proc->_df
#define _OF proc->_of
    
#define STACK proc->stack
    
    typedef struct _Processeur{
        LinkedList* stack;
        
        Registre*	rax;
        Registre*   eax;    // ne pas détruire
        Registre*   ax;     // ne pas détruire
        Registre*   ah;     // ne pas détruire
        Registre*   al;     // ne pas détruire
        
        Registre*	rbx;
        Registre*	ebx;    // ne pas détruire
        Registre*	bx;     // ne pas détruire
        Registre*	bh;     // ne pas détruire
        Registre*	bl;     // ne pas détruire
        
        Registre*	rcx;
        Registre*	ecx;    // ne pas détruire
        Registre*	cx;     // ne pas détruire
        Registre*	ch;     // ne pas détruire
        Registre*	cl;     // ne pas détruire
        
        Registre*	rdx;
        Registre*	edx;    // ne pas détruire
        Registre*	dx;     // ne pas détruire
        Registre*	dh;     // ne pas détruire
        Registre*	dl;     // ne pas détruire
        
        Registre*	rdi;
        Registre*   edi;    // ne pas détruire
        
        Registre*	rsi;
        Registre*   esi;    // ne pas détruire
        
        Registre*	rbp;    // souvent une copie de rsp
        Registre*   ebp;    // ne pas détruire
        
        Registre*	rsp;    // stack pointer
        Registre*   esp;    // ne pas détruire
        
        Registre*	rip;
        Registre*   eip;    // ne pas détruire
        
        Registre*	rflags;
        Registre*   eflags; // ne pas détruire
        
        Registre*	r8;
        Registre*	r9;
        Registre*	r10;
        Registre*	r11;
        Registre*	r12;
        Registre*	r13;
        Registre*	r14;
        Registre*	r15;
        
        Registre*	cs;     // tenir compte des 32 bits et 64 bits
        Registre*   ds;     // tenir compte des 32 bits et 64 bits
        Registre*   ss;     // tenir compte des 32 bits et 64 bits
        Registre*   es;     // tenir compte des 32 bits et 64 bits
        Registre*	fs;     // tenir compte des 32 bits et 64 bits
        Registre*	gs;     // tenir compte des 32 bits et 64 bits
        
        int8_t    _cf;     // carry flag (retenue)
        int8_t    _pf;     // parity flag
        int8_t    _af;     // auxiliary flag (retenue aux)
        int8_t    _zf;     // zero flag
        int8_t    _sf;     // sign flag
        int8_t    _tf;     // single step flag (debugage)
        int8_t    _if;     // interrupt flag
        int8_t    _df;     // direction flag (chaine)
        int8_t    _of;     // overflow flag
    }Processeur;
    
    Processeur* newProcesseur();
    void terminateProcesseur(Processeur* proc);
    
    /*------------ Modifs registres ------------*/
    
    void _and(Processeur* proc, int lenInstr, Registre* a, Registre* b);
    void _add(Processeur* proc, int lenInstr, Registre* destination, Registre* masque); //{destion} &= {masque}
    void _lea(Processeur* proc, int lenInstr, Registre* a, Registre* b);
    void _shl(Processeur* proc, int lenInstr, Registre* reg, Registre* val); // shift left
    void _shr(Processeur* proc, int lenInstr, Registre* reg, Registre* val);
    void _mov(Processeur* proc, int lenInstr, Registre* a, Registre* b);
    void _sub(Processeur* proc, int lenInstr, Registre* a, Registre* b);
    void _xor(Processeur* proc, int lenInstr, Registre* a, Registre* b); //xor
    
    
    /*---------------- sauts -------------------*/
    
    void _call(Processeur* proc, int lenInstr, Registre* adresse);
    void _jmp(Processeur* proc, Registre* adresse);
    void _jne(Processeur* proc, int lenInstr, Registre* adresse);
    void _ja(Processeur* proc, int lenInstr, Registre* adresse);
    void _jb(Processeur* proc, int lenInstr, Registre* adresse);
    void _jbe(Processeur* proc, int lenInstr, Registre* adresse);
    void _je(Processeur* proc, int lenInstr, Registre* adresse);
    void _jg(Processeur* proc, int lenInstr, Registre* adresse);
    void _jle(Processeur* proc, int lenInstr, Registre* adresse);
    void _ret(Processeur* proc);
    
    /*----------------- flags ------------------*/
    
    void _cmp(Processeur* proc, int lenInstr, Registre* a, Registre* b);
    
    /*------------------ pile ------------------*/
    
    void _push(Processeur* proc, int lenInstr, Registre* a);
    void _pop(Processeur* proc, int lenInstr, Registre* reg);
    
#ifdef __cplusplus
}
#endif

#endif


















