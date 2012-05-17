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
    _AL = newRegistreFeuille(8, 0);
    _AH = newRegistreFeuille(8, 0);
    _AX = newRegistreDeuxFils(_AH, _AL);
    _EAX = newRegistreFilsUnique(_AX);
    _RAX = newRegistreFilsUnique(_EAX);
    
    _BL = newRegistreFeuille(8, 0);
    _BH = newRegistreFeuille(8, 0);
    _BX = newRegistreDeuxFils(_BH, _BL);
    _EBX = newRegistreFilsUnique(_BX);
    _RBX = newRegistreFilsUnique(_EBX);
    
    _CL = newRegistreFeuille(8, 0);
    _CH = newRegistreFeuille(8, 0);
    _CX = newRegistreDeuxFils(_CH, _CL);
    _ECX = newRegistreFilsUnique(_CX);
    _RCX = newRegistreFilsUnique(_ECX);
    
    _DL = newRegistreFeuille(8, 0);
    _DH = newRegistreFeuille(8, 0);
    _DX = newRegistreDeuxFils(_DH, _DL);
    _EDX = newRegistreFilsUnique(_DX);
    _RDX = newRegistreFilsUnique(_EDX);
    
    _DI = newRegistreFeuille(16, 0);
    _EDI = newRegistreFilsUnique(_DI);
    _RDI = newRegistreFilsUnique(_EDI);
    
    _SI = newRegistreFeuille(16, 0);
    _ESI = newRegistreFilsUnique(_SI);
    _RSI = newRegistreFilsUnique(_ESI);
    
    _BP = newRegistreFeuille(16, 0);
    _EBP = newRegistreFilsUnique(_BP);
    _RBP = newRegistreFilsUnique(_EBP);
    
    _SP = newRegistreFeuille(16, 0);
    _ESP = newRegistreFilsUnique(_SP);
    _RSP = newRegistreFilsUnique(_ESP);
    
    _IP = newRegistreFeuille(16, 0);
    _EIP = newRegistreFilsUnique(_IP);
    _RIP = newRegistreFilsUnique(_EIP);
    
    _FLAGS = newRegistreFeuille(16, 0);
    _EFLAGS = newRegistreFilsUnique(_FLAGS);
    _RFLAGS = newRegistreFilsUnique(_EFLAGS);
    
    _R8 = newRegistreFeuille(64, 0);
    _R9 = newRegistreFeuille(64, 0);
    _R10 = newRegistreFeuille(64, 0);
    _R11 = newRegistreFeuille(64, 0);
    _R12 = newRegistreFeuille(64, 0);
    _R13 = newRegistreFeuille(64, 0);
    _R14 = newRegistreFeuille(64, 0);
    _R15 = newRegistreFeuille(64, 0);
    
    _CS = newRegistreFeuille(64, 0);
    _DS = newRegistreFeuille(64, 0);
    _SS = newRegistreFeuille(64, 0);
    _ES = newRegistreFeuille(64, 0);
    _FS = newRegistreFeuille(64, 0);
    _GS = newRegistreFeuille(64, 0);
    
    _STACK = newLinkedList();
    
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
    
    
}