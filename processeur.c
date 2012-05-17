//
//  processeur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "processeur.h"
#include "_macro_Build.h"


Processeur* newProcesseur(){
    Processeur* proc = malloc(sizeof(Processeur));
    _AL     =   newRegistreFeuille(8, 0);
    _AH     =   newRegistreFeuille(8, 0);
    _AX     =   newRegistreDeuxFils(_AH, _AL);
    _EAX    =   newRegistreFilsUnique(_AX);
    _RAX    =   newRegistreFilsUnique(_EAX);
    
    _BL     =   newRegistreFeuille(8, 0);
    _BH     =   newRegistreFeuille(8, 0);
    _BX     =   newRegistreDeuxFils(_BH, _BL);
    _EBX    =   newRegistreFilsUnique(_BX);
    _RBX    =   newRegistreFilsUnique(_EBX);
    
    _CL     =   newRegistreFeuille(8, 0);
    _CH     =   newRegistreFeuille(8, 0);
    _CX     =   newRegistreDeuxFils(_CH, _CL);
    _ECX    =   newRegistreFilsUnique(_CX);
    _RCX    =   newRegistreFilsUnique(_ECX);
    
    _DL     =   newRegistreFeuille(8, 0);
    _DH     =   newRegistreFeuille(8, 0);
    _DX     =   newRegistreDeuxFils(_DH, _DL);
    _EDX    =   newRegistreFilsUnique(_DX);
    _RDX    =   newRegistreFilsUnique(_EDX);

    _DIL    =   newRegistreFeuille(8, 0);
    _DI     =   newRegistreFilsUnique(_DIL);
    _EDI    =   newRegistreFilsUnique(_DI);
    _RDI    =   newRegistreFilsUnique(_EDI);
    
    _SIL    =   newRegistreFeuille(8, 0);
    _SI     =   newRegistreFilsUnique(_SIL);
    _ESI    =   newRegistreFilsUnique(_SI);
    _RSI    =   newRegistreFilsUnique(_ESI);
    
    _BPL    =   newRegistreFeuille(8, 0);
    _BP     =   newRegistreFilsUnique(_BPL);
    _EBP    =   newRegistreFilsUnique(_BP);
    _RBP    =   newRegistreFilsUnique(_EBP);
    
    _SPL    =   newRegistreFeuille(8, 0);
    _SP     =   newRegistreFilsUnique(_SPL);
    _ESP    =   newRegistreFilsUnique(_SP);
    _RSP    =   newRegistreFilsUnique(_ESP);
    
    _IP     =   newRegistreFeuille(16, 0);
    _EIP    =   newRegistreFilsUnique(_IP);
    _RIP    =   newRegistreFilsUnique(_EIP);
    
    _FLAGS  =   newRegistreFeuille(16, 0);
    _EFLAGS =   newRegistreFilsUnique(_FLAGS);
    _RFLAGS =   newRegistreFilsUnique(_EFLAGS);
    
    _R8B    =   newRegistreFeuille(8, 0);
    _R8W    =   newRegistreFilsUnique(_R8B);
    _R8D    =   newRegistreFilsUnique(_R8W);
    _R8     =   newRegistreFilsUnique(_R8D);
    
	_R9B    =   newRegistreFeuille(9, 0);
    _R9W    =   newRegistreFilsUnique(_R9B);
    _R9D    =   newRegistreFilsUnique(_R9W);
    _R9     =   newRegistreFilsUnique(_R9D);
    
	_R10B   =   newRegistreFeuille(10, 0);
	_R10W   =   newRegistreFilsUnique(_R10B);
	_R10D   =   newRegistreFilsUnique(_R10W);
	_R10    =   newRegistreFilsUnique(_R10D);
    
	_R11B   =   newRegistreFeuille(11, 0);
	_R11W   =   newRegistreFilsUnique(_R11B);
	_R11D   =   newRegistreFilsUnique(_R11W);
	_R11    =   newRegistreFilsUnique(_R11D);
    
	_R12B   =   newRegistreFeuille(12, 0);
	_R12W   =   newRegistreFilsUnique(_R12B);
	_R12D   =   newRegistreFilsUnique(_R12W);
	_R12    =   newRegistreFilsUnique(_R12D);
    
	_R13B   =   newRegistreFeuille(13, 0);
	_R13W   =   newRegistreFilsUnique(_R13B);
	_R13D   =   newRegistreFilsUnique(_R13W);
	_R13    =   newRegistreFilsUnique(_R13D);
    
	_R14B   =   newRegistreFeuille(14, 0);
	_R14W   =   newRegistreFilsUnique(_R14B);
	_R14D   =   newRegistreFilsUnique(_R14W);
	_R14    =   newRegistreFilsUnique(_R14D);
    
	_R15B   =   newRegistreFeuille(15, 0);
	_R15W   =   newRegistreFilsUnique(_R15B);
	_R15D   =   newRegistreFilsUnique(_R15W);
	_R15    =   newRegistreFilsUnique(_R15D);
    
    _CS     =   newRegistreFeuille(64, 0);
    _DS     =   newRegistreFeuille(64, 0);
    _SS     =   newRegistreFeuille(64, 0);
    _ES     =   newRegistreFeuille(64, 0);
    _FS     =   newRegistreFeuille(64, 0);
    _GS     =   newRegistreFeuille(64, 0);
    
    _STACK  =   newLinkedList();
    
    return proc;
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
    free(proc);
}

int incluDans(Processeur* p1, Processeur* p2){
    if (getClasse(p1->rax)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->rax) != getValeur(p2->rax)) {
        return 0;
    }
    if (getClasse(p1->eax)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->eax) != getValeur(p2->eax)) {
        return 0;
    }
    if (getClasse(p1->ax)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->ax) != getValeur(p2->ax)) {
        return 0;
    }
    if (getClasse(p1->ah)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->ah) != getValeur(p2->ah)) {
        return 0;
    }
    if (getClasse(p1->al)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->al) != getValeur(p2->al)) {
        return 0;
    }
    
    if (getClasse(p1->rbx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->rbx) != getValeur(p2->rbx)) {
        return 0;
    }
    if (getClasse(p1->ebx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->ebx) != getValeur(p2->ebx)) {
        return 0;
    }
    if (getClasse(p1->bx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->bx) != getValeur(p2->bx)) {
        return 0;
    }
    if (getClasse(p1->bh)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->bh) != getValeur(p2->bh)) {
        return 0;
    }
    if (getClasse(p1->bl)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->bl) != getValeur(p2->bl)) {
        return 0;
    }
    
    if (getClasse(p1->rcx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->rcx) != getValeur(p2->rcx)) {
        return 0;
    }
    if (getClasse(p1->ecx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->ecx) != getValeur(p2->ecx)) {
        return 0;
    }
    if (getClasse(p1->cx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->cx) != getValeur(p2->cx)) {
        return 0;
    }
    if (getClasse(p1->ch)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->ch) != getValeur(p2->ch)) {
        return 0;
    }
    if (getClasse(p1->cl)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->cl) != getValeur(p2->cl)) {
        return 0;
    }
    
    if (getClasse(p1->rdx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->rdx) != getValeur(p2->rdx)) {
        return 0;
    }
    if (getClasse(p1->edx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->edx) != getValeur(p2->edx)) {
        return 0;
    }
    if (getClasse(p1->dx)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->dx) != getValeur(p2->dx)) {
        return 0;
    }
    if (getClasse(p1->dh)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->dh) != getValeur(p2->dh)) {
        return 0;
    }
    if (getClasse(p1->dl)!=VALEUR_NON_DEFINIE ||
        getValeur(p1->dl) != getValeur(p2->dl)) {
        return 0;
    }
    return 1;
}