//
//  processeur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "processeur.h"
#include "_macro_Build.h"


Processeur* newProcesseur(void){
    Processeur* proc = malloc(sizeof(Processeur));
    _AL     =   newRegistreFeuille(8);
    _AH     =   newRegistreFeuille(8);
    _AX     =   newRegistreDeuxFils(_AH, _AL);
    _EAX    =   newRegistreFilsUnique(_AX);
    _RAX    =   newRegistreFilsUnique(_EAX);
    
    _BL     =   newRegistreFeuille(8);
    _BH     =   newRegistreFeuille(8);
    _BX     =   newRegistreDeuxFils(_BH, _BL);
    _EBX    =   newRegistreFilsUnique(_BX);
    _RBX    =   newRegistreFilsUnique(_EBX);
    
    _CL     =   newRegistreFeuille(8);
    _CH     =   newRegistreFeuille(8);
    _CX     =   newRegistreDeuxFils(_CH, _CL);
    _ECX    =   newRegistreFilsUnique(_CX);
    _RCX    =   newRegistreFilsUnique(_ECX);
    
    _DL     =   newRegistreFeuille(8);
    _DH     =   newRegistreFeuille(8);
    _DX     =   newRegistreDeuxFils(_DH, _DL);
    _EDX    =   newRegistreFilsUnique(_DX);
    _RDX    =   newRegistreFilsUnique(_EDX);

    _DIL    =   newRegistreFeuille(8);
    _DI     =   newRegistreFilsUnique(_DIL);
    _EDI    =   newRegistreFilsUnique(_DI);
    _RDI    =   newRegistreFilsUnique(_EDI);
    
    _SIL    =   newRegistreFeuille(8);
    _SI     =   newRegistreFilsUnique(_SIL);
    _ESI    =   newRegistreFilsUnique(_SI);
    _RSI    =   newRegistreFilsUnique(_ESI);
    
    _BPL    =   newRegistreFeuille(8);
    _BP     =   newRegistreFilsUnique(_BPL);
    _EBP    =   newRegistreFilsUnique(_BP);
    _RBP    =   newRegistreFilsUnique(_EBP);
    
    _SPL    =   newRegistreFeuille(8);
    _SP     =   newRegistreFilsUnique(_SPL);
    _ESP    =   newRegistreFilsUnique(_SP);
    _RSP    =   newRegistreFilsUnique(_ESP);
    
    _IP     =   newRegistreFeuille(16);
    _EIP    =   newRegistreFilsUnique(_IP);
    _RIP    =   newRegistreFilsUnique(_EIP);
    
    _FLAGS  =   newRegistreFeuille(16);
    _EFLAGS =   newRegistreFilsUnique(_FLAGS);
    _RFLAGS =   newRegistreFilsUnique(_EFLAGS);
    
    _R8B    =   newRegistreFeuille(8);
    _R8W    =   newRegistreFilsUnique(_R8B);
    _R8D    =   newRegistreFilsUnique(_R8W);
    _R8     =   newRegistreFilsUnique(_R8D);
    
	_R9B    =   newRegistreFeuille(8);
    _R9W    =   newRegistreFilsUnique(_R9B);
    _R9D    =   newRegistreFilsUnique(_R9W);
    _R9     =   newRegistreFilsUnique(_R9D);
    
	_R10B   =   newRegistreFeuille(8);
	_R10W   =   newRegistreFilsUnique(_R10B);
	_R10D   =   newRegistreFilsUnique(_R10W);
	_R10    =   newRegistreFilsUnique(_R10D);
    
	_R11B   =   newRegistreFeuille(8);
	_R11W   =   newRegistreFilsUnique(_R11B);
	_R11D   =   newRegistreFilsUnique(_R11W);
	_R11    =   newRegistreFilsUnique(_R11D);
    
	_R12B   =   newRegistreFeuille(8);
	_R12W   =   newRegistreFilsUnique(_R12B);
	_R12D   =   newRegistreFilsUnique(_R12W);
	_R12    =   newRegistreFilsUnique(_R12D);
    
	_R13B   =   newRegistreFeuille(8);
	_R13W   =   newRegistreFilsUnique(_R13B);
	_R13D   =   newRegistreFilsUnique(_R13W);
	_R13    =   newRegistreFilsUnique(_R13D);
    
	_R14B   =   newRegistreFeuille(8);
	_R14W   =   newRegistreFilsUnique(_R14B);
	_R14D   =   newRegistreFilsUnique(_R14W);
	_R14    =   newRegistreFilsUnique(_R14D);
    
	_R15B   =   newRegistreFeuille(8);
	_R15W   =   newRegistreFilsUnique(_R15B);
	_R15D   =   newRegistreFilsUnique(_R15W);
	_R15    =   newRegistreFilsUnique(_R15D);
    
    _CS     =   newRegistreFeuille(64);
    _DS     =   newRegistreFeuille(64);
    _SS     =   newRegistreFeuille(64);
    _ES     =   newRegistreFeuille(64);
    _FS     =   newRegistreFeuille(64);
    _GS     =   newRegistreFeuille(64);
    
    for (int i =0; i<NOMBRE_FLAGS; i++) {
        proc->tabFlags[i] = FLAG_NON_DEFINI;
    }
    
//    _CF     =   FLAG_BAS;
//    _PF     =   FLAG_BAS;
//    _AF     =   FLAG_BAS;
//    _ZF     =   FLAG_BAS;
//    _SF     =   FLAG_BAS;
//    _TF     =   FLAG_BAS;
//    _IF     =   FLAG_BAS;
//    _DF     =   FLAG_BAS;
//    _OF     =   FLAG_BAS;
    
    _STACK  =   newLinkedList();
    
    proc->delta = DELTA_BAISSE;
    
    return proc;
}

Processeur* newProcesseurCopy(const Processeur* p){
    Processeur* res = newProcesseur();
    res->delta = p->delta;
    res->stack = newLinkedListCopy(p->stack);
    for (int i = 0; i<NOMBRE_REGISTRES; i++) {
        res->tabRegistre[i]->classe = p->tabRegistre[i]->classe;
        res->tabRegistre[i]->valeur = p->tabRegistre[i]->valeur;
    }
    for (int i = 0; i<NOMBRE_FLAGS; i++) {
        res->tabFlags[i] = res->tabFlags[i];
    }
    return res;
}

void terminateProcesseur(Processeur* proc){
    terminateRegistre(_RAX);
    terminateRegistre(_RBX);
    terminateRegistre(_RCX);
    terminateRegistre(_RDX);
    terminateRegistre(_RDI);
    terminateRegistre(_RSI);
    terminateRegistre(_RSP);
    terminateRegistre(_RBP);
    terminateRegistre(_RIP);
    terminateRegistre(_RFLAGS);
    terminateRegistre(_R8);
    terminateRegistre(_R9);
    terminateRegistre(_R10);
    terminateRegistre(_R11);
    terminateRegistre(_R12);
    terminateRegistre(_R13);
    terminateRegistre(_R14);
    terminateRegistre(_R15);
    terminateRegistre(_CS);
    terminateRegistre(_DS);
    terminateRegistre(_SS);
    terminateRegistre(_ES);
    terminateRegistre(_FS);
    terminateRegistre(_GS);
    terminateLinkedList(proc->stack);
    free(proc);
}

int incluDans(const Processeur* p1, const Processeur* p2, Fichier* fichierlog){
    char temp[MAX_BUFFER];
    
    int res = EGAL;
    
    if (p1->delta == DELTA_LEVE && p2->delta == DELTA_BAISSE) {
        sprintf(temp, "premiere non inclusion\n");
        pushlog(fichierlog, temp);
        return NON_INCLUS;
    }
    
    for (int i = 0; i<NOMBRE_REGISTRES; i++) {
        Registre* r1 = p1->tabRegistre[i];
        Registre* r2 = p2->tabRegistre[i];
        if (getClasse(r1) != CLASSE_NON_DEFINIE && (
            getClasse(r2) == CLASSE_NON_DEFINIE ||
            getValeur(r1) != getValeur(r2))) {
            sprintf(temp, "non inclus par registre\n");
            pushlog(fichierlog, temp);
            sprintf(temp, "\t r1 = (%d, %lu)\n", getClasse(r1), (long) getValeur(r1));
            pushlog(fichierlog, temp);
            sprintf(temp, "\t r2 = (%d, %lu)\n", getClasse(r2), (long) getValeur(r2));
            pushlog(fichierlog, temp);
            return NON_INCLUS;
        }
    }
    
	for (int i = 0; i<NOMBRE_FLAGS; i++) {
        int f1 = p1->tabFlags[i];
        int f2 = p2->tabFlags[i];
        if (f1 != FLAG_NON_DEFINI && (f2 == FLAG_NON_DEFINI || f1 != f2)) {            
            sprintf(temp,"non inclus par drapeau\n");
            pushlog(fichierlog, temp);
            sprintf(temp, "\t f1 = %d\n", f1);
            pushlog(fichierlog, temp);
            sprintf(temp, "\t f2 = %d\n", f2);
            pushlog(fichierlog, temp);
            return NON_INCLUS;
        }
    }
    
	if (p1->stack != PILE_NON_DEFINIE && 
        (p2->stack == PILE_NON_DEFINIE ||
         compare(p1->stack, p2->stack) != 0)) {
            sprintf(temp, "non inclus par pile\n");
            pushlog(fichierlog, temp);
            return NON_INCLUS;
        }
	return res;
}

/*
 * Peut être optimisée en faisant la différence entre les types de registres
 */

void inter(Processeur* p1, const Processeur* p2){
    
    if (p1->delta == DELTA_LEVE) {
        p1->delta = p2->delta;
    }
    
    for (int i = 0; i<NOMBRE_REGISTRES; i++) {
        Registre* r1 = p1->tabRegistre[i];
        Registre* r2 = p2->tabRegistre[i];
        if (getClasse(r1) != CLASSE_NON_DEFINIE && (
            getClasse(r2) == CLASSE_NON_DEFINIE ||
            getValeur(r1) != getValeur(r2))) {
            r1->classe = CLASSE_NON_DEFINIE;
        }
    }
    
    for (int i = 0; i<NOMBRE_FLAGS; i++) {
        int f1 = p1->tabFlags[i];
        int f2 = p2->tabFlags[i];
        if (f1 != FLAG_NON_DEFINI && (f2 == FLAG_NON_DEFINI || f1 != f2)) {
//            printf("passage de f1 de %d à %d\n", f1, FLAG_NON_DEFINI);
            p1->tabFlags[i] = FLAG_NON_DEFINI;
        }
    }
    
    if (p1->stack != PILE_NON_DEFINIE && 
        (p2->stack == PILE_NON_DEFINIE ||
         compare(p1->stack, p2->stack) != 0)) {
            terminateLinkedList(p1->stack);
            p1->stack = PILE_NON_DEFINIE;
        }
}