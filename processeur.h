//
//  processeur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_processeur_h
#define desassembleur_processeur_h

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
	int64	rip;    // Registre IP
	int64	rflags; // Registre de flags
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

void add(Processeur* proc, int64* destination, int64* masque); //{destion} &= {masque}
void move(Processeur* proc, int64* dest, int64* source);
void lea(Processeur* proc, int64* a, int64* b);
void shl(Processeur* proc, int64* reg, int64* val); // shift left
void shr(Processeur* proc, int64* reg, int64* val);
void mov(Processeur* proc, int64* a, int64* b);
void sub(Processeur* proc, int64* a, int64* b);
void xr(Processeur* proc, int64* a, int64* b); //xor


/*---------------- sauts -------------------*/

void call(Processeur* proc, int64* adresse);
void jmp(Processeur* proc, int64);
void jne(Processeur* proc, int64);

/*----------------- flags ------------------*/

void cmp(Processeur* proc, int64* a, int64* b);

/*------------------ pile ------------------*/

void push(Processeur* proc, int64* a);
void pop(Processeur* proc, int64* reg);

#endif


















