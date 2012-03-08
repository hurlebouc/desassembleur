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

typedef unsigned long long int64;

typedef struct _Processeur{
    LinkedList* pileAppel;
    int64	rax;
	int64	rbx;
	int64	rcx;
	int64	rdx;
	int64	rdi;
	int64	rsi;
	int64	rbp;    // souvent une copie de rsp
	int64	rsp;    // stack pointer
	int64	r8;
	int64	r9;
	int64	r10;
	int64	r11;
	int64	r12;
	int64	r13;
	int64	r14;
	int64	r15;
	int64	rip;    // registre IP
	int64	rflags; // registre de flags
	int64	cs;
	int64	fs;
	int64	gs;
    char    CF;     // carry flag (retenue)
    char    PF;     // parity flag
    char    AF;     // auxiliary flag (retenue aux)
    char    ZF;     // zero flag
    char    SF;     // sign flag
    char    TF;     // single step flag (debugage)
    char    IF;     // interrupt flag
    char    DF;     // direction flag (chaine)
    char    OF;     // overfolw flag
}Processeur;

/*------------ Modifs registres ------------*/

void _and(int64 a, int64 b);
void _add(Processeur* proc, int64* destination, int64* masque); //{destion} &= {masque}
void _move(int64* dest, int64* source);
void _lea(Processeur* proc, int len, int64* a, int64* b);
void _shl(Processeur* proc, int len, int64* reg, int64* val); // shift left
void _shr(Processeur* proc, int len, int64* reg, int64* val);
void _mov(Processeur* proc, int len, int64* a, int64* b);
void _sub(Processeur* proc, int len, int64* a, int64* b);
void _xor(Processeur* proc, int len, int64* a, int64* b); //xor


/*---------------- sauts -------------------*/

void _call(Processeur* proc, int len, int64* adresse);
void _jmp(Processeur* proc, int64* adresse);
void _jne(Processeur* proc, int len, int64* adresse);
void _ja(Processeur* proc, int len, int64* adresse);
void _jb(Processeur* proc, int len, int64* adresse);
void _jbe(Processeur* proc, int len, int64* adresse);
void _je(Processeur* proc, int len, int64* adresse);
void _jg(Processeur* proc, int len, int64* adresse);
void _jle(Processeur* proc, int len, int64* adresse);
void _ret(Processeur* proc);

/*----------------- flags ------------------*/

void _cmp(Processeur* proc, int len, int64* a, int64* b);

/*------------------ pile ------------------*/

void _push(Processeur* proc, int len, int64* a);
void _pop(Processeur* proc, int len, int64* reg);

#ifdef __cplusplus
}
#endif
    
#endif


















