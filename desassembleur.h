//
//  desassembleur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_desassembleur_h
#define desassembleur_desassembleur_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "Fichier.h"
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine.h"
//#include "LoaderMach.h"
//#include "loaderElf.h"

typedef struct _Processeur{
    unsigned long long	rax;
	unsigned long long	rbx;
	unsigned long long	rcx;
	unsigned long long	rdx;
	unsigned long long	rdi;
	unsigned long long	rsi;
	unsigned long long	rbp;
	unsigned long long	rsp;
	unsigned long long	r8;
	unsigned long long	r9;
	unsigned long long	r10;
	unsigned long long	r11;
	unsigned long long	r12;
	unsigned long long	r13;
	unsigned long long	r14;
	unsigned long long	r15;
	unsigned long long	rip;
	unsigned long long	rflags;
	unsigned long long	cs;
	unsigned long long	fs;
	unsigned long long	gs;
}Processeur;

typedef struct _desassembleur{
    Processeur* proc;
    DISASM* disasm;
    unsigned long debutVirtuel;    // debut virtuel du bloc
}desasembleur;



desasembleur* newDesassembleur(desasembleur* desas);
void terminateDesassembleur(desasembleur* desas);

#endif
