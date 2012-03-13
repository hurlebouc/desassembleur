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
        
        int8_t    CF;     // carry flag (retenue)
        int8_t    PF;     // parity flag
        int8_t    AF;     // auxiliary flag (retenue aux)
        int8_t    ZF;     // zero flag
        int8_t    SF;     // sign flag
        int8_t    TF;     // single step flag (debugage)
        int8_t    IF;     // interrupt flag
        int8_t    DF;     // direction flag (chaine)
        int8_t    OF;     // overfolw flag
    }Processeur;
    
    Processeur* newProcesseur();
    void terminateProcesseur(Processeur* proc);
    
    /*------------ Modifs registres ------------*/
    
    void _and(Processeur* proc, Registre* a, Registre* b);
    void _add(Processeur* proc, Registre* destination, Registre* masque); //{destion} &= {masque}
    void _move(Registre* dest, Registre* source);
    void _lea(Processeur* proc, int len, Registre* a, Registre* b);
    void _shl(Processeur* proc, int len, Registre* reg, Registre* val); // shift left
    void _shr(Processeur* proc, int len, Registre* reg, Registre* val);
    void _mov(Processeur* proc, int len, Registre* a, Registre* b);
    void _sub(Processeur* proc, int len, Registre* a, Registre* b);
    void _xor(Processeur* proc, int len, Registre* a, Registre* b); //xor
    
    
    /*---------------- sauts -------------------*/
    
    void _call(Processeur* proc, int len, Registre* adresse);
    void _jmp(Processeur* proc, Registre* adresse);
    void _jne(Processeur* proc, int len, Registre* adresse);
    void _ja(Processeur* proc, int len, Registre* adresse);
    void _jb(Processeur* proc, int len, Registre* adresse);
    void _jbe(Processeur* proc, int len, Registre* adresse);
    void _je(Processeur* proc, int len, Registre* adresse);
    void _jg(Processeur* proc, int len, Registre* adresse);
    void _jle(Processeur* proc, int len, Registre* adresse);
    void _ret(Processeur* proc);
    
    /*----------------- flags ------------------*/
    
    void _cmp(Processeur* proc, int len, Registre* a, Registre* b);
    
    /*------------------ pile ------------------*/
    
    void _push(Processeur* proc, int len, Registre* a);
    void _pop(Processeur* proc, int len, Registre* reg);
    
#ifdef __cplusplus
}
#endif

#endif


















