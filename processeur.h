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
    
#define CF_POWER 1
#define PF_POWER 4 
#define AF_POWER 16
#define ZF_POWER 64
#define SF_POWER 128
#define TF_POWER 256
#define IF_POWER 512
#define DF_POWER 1024
#define OF_POWER 2048
    
    enum armement_flag {
        LEVE,
        BAISSE,
    };
    
    typedef struct _Processeur{
        LinkedList* stack;
        
        Registre*	rax;
        Registre*   eax;   
        Registre*   ax;    
        Registre*   ah;    
        Registre*   al;    
        
        Registre*	rbx;
        Registre*	ebx;   
        Registre*	bx;    
        Registre*	bh;    
        Registre*	bl;    
        
        Registre*	rcx;
        Registre*	ecx;   
        Registre*	cx;    
        Registre*	ch;    
        Registre*	cl;    
        
        Registre*	rdx;
        Registre*	edx;   
        Registre*	dx;    
        Registre*	dh;    
        Registre*	dl;    
        
        Registre*	rdi;
        Registre*   edi;   
        Registre*   di;
        Registre*   dil;
        
        Registre*	rsi;
        Registre*   esi;  
        Registre*   si;
        Registre*   sil;
        
        Registre*	rbp;    // souvent une copie de rsp
        Registre*   ebp;
        Registre*   bp;
        Registre*   bpl;
        
        Registre*	rsp;    // stack pointer
        Registre*   esp;
        Registre*   sp;
        Registre*   spl;
        
        Registre*	rip;
        Registre*   eip;
        Registre*   ip;
        
        Registre*	rflags;
        Registre*   eflags;
        Registre*   flags;
        
        Registre*	r8;
        Registre*   r8d;
        Registre*   r8w;
        Registre*   r8b;
        
        Registre*	r9;
        Registre*   r9d;
        Registre*   r9w;
        Registre*   r9b;
        
        Registre*	r10;
        Registre*   r10d;
        Registre*   r10w;
        Registre*   r10b;
        
        Registre*	r11;
        Registre*   r11d;
        Registre*   r11w;
        Registre*   r11b;
        
		Registre*	r12;
        Registre*   r12d;
        Registre*   r12w;
        Registre*   r12b;
        
		Registre*	r13;
        Registre*   r13d;
        Registre*   r13w;
        Registre*   r13b;
        
		Registre*	r14;
        Registre*   r14d;
        Registre*   r14w;
        Registre*   r14b;
        
		Registre*	r15;
        Registre*   r15d;
        Registre*   r15w;
        Registre*   r15b;
                
        Registre*	cs;     // tenir compte des 32 bits et 64 bits
        Registre*   ds;     // tenir compte des 32 bits et 64 bits
        Registre*   ss;     // tenir compte des 32 bits et 64 bits
        Registre*   es;     // tenir compte des 32 bits et 64 bits
        Registre*	fs;     // tenir compte des 32 bits et 64 bits
        Registre*	gs;     // tenir compte des 32 bits et 64 bits
        
        int8_t    _cf;     // carry flag (retenue) ////////////////
        int8_t    _pf;     // parity flag
        int8_t    _af;     // auxiliary flag (retenue aux) ////////
        int8_t    _zf;     // zero flag
        int8_t    _sf;     // sign flag
        int8_t    _tf;     // trap flag (debugage)
        int8_t    _if;     // interrupt flag
        int8_t    _df;     // direction flag (chaine)
        int8_t    _of;     // overflow flag ///////////////////////
    }Processeur;
    
    Processeur* newProcesseur();
    void terminateProcesseur(Processeur* proc);
    
    
    /* ----------------------------- PROPAGATION --------------------------- */
    
    int incluDans(Processeur* p1, Processeur* p2);
    int inter(Processeur*, Processeur*);
    
#ifdef __cplusplus
}
#endif

#endif


















