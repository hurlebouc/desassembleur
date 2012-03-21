//
//  processeur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "processeur.h"



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
    
    _EDI = newRegistreFeuille(32, 0);
    _RDI = newRegistreFilsUnique(_EDI);
    
    _ESI = newRegistreFeuille(32, 0);
    _RSI = newRegistreFilsUnique(_ESI);
    
    _EBP = newRegistreFeuille(32, 0);
    _RBP = newRegistreFilsUnique(_EBP);
    
    _ESP = newRegistreFeuille(32, 0);
    _RSP = newRegistreFilsUnique(_ESP);
    
    _EIP = newRegistreFeuille(32, 0);
    _RIP = newRegistreFilsUnique(_EIP);
    
    _EFLAGS = newRegistreFeuille(32, 0);
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
    
    STACK = newLinkedList();
    
    return proc;
}

void terminateProcesseur(Processeur* proc){
    terminateRegistre(_RAX);
    terminateRegistre(_RBX);
    terminateRegistre(_RCX);
}

void _call(Processeur* proc, int len, Registre* adresse){
    addFirstLL(STACK, (void *) (_RIP + len));
    copieVal(_RIP, adresse);
}

void _jmp(Processeur* proc, Registre* adresse){
    copieVal(_RIP, adresse);
}

void _jne(Processeur* proc, int len, Registre* adresse){
    if (!_ZF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _je(Processeur* proc, int len, Registre* adresse){
    if (_ZF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _ja(Processeur* proc, int len, Registre* adresse){
    if (!_ZF && !_CF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _jb(Processeur* proc, int len, Registre* adresse){
    if (_CF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _jbe(Processeur* proc, int len, Registre* adresse){
    if (_ZF || _CF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _jg(Processeur* proc, int len, Registre* adresse){
    if (!_ZF && _SF == _OF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _jle(Processeur* proc, int len, Registre* adresse){
    if (_ZF || _SF != _OF) {
        _jmp(proc, adresse);
    } else {
        incr(_RIP, len);
    }
}

void _and(Processeur* proc, int lenInstr, Registre* destination, Registre* masque){
    uint64_t dest = getValeur(destination);
    uint64_t mask = getValeur(masque);
    dest &= mask;
    setValeur(destination, dest);
    
    
    // peut-être encore des choses à faire sur le registre de flags
}

void _mov(Processeur* proc, int lenInstr, Registre* dest, Registre* source){
    
}









