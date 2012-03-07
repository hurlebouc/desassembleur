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
	int64	rbp;
	int64	rsp;
	int64	r8;
	int64	r9;
	int64	r10;
	int64	r11;
	int64	r12;
	int64	r13;
	int64	r14;
	int64	r15;
	int64	rip;
	int64	rflags;
	int64	cs;
	int64	fs;
	int64	gs;
    char    CF;
    char    PF;
    char    AF;
    char    ZF;
    char    SF;
    char    TF;
    char    IF;
    char    DF;
    char    OF;
}Processeur;

/*------------ Modifs registres ------------*/

void add(Processeur* proc, int64* destination, int64* masque); //{destion} &= {masque}
void move(Processeur* proc, int64* dest, int64* source);
void lea(Processeur* proc, int64* a, int64* b);

/*---------------- sauts -------------------*/

void call(Processeur* proc, int64* adresse);

/*----------------- flags ------------------*/

void cmp(Processeur* proc, int64* a, int64* b);

/*------------------ pile ------------------*/

void push(Processeur* proc, int64* a);


#endif


















