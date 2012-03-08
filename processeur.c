//
//  processeur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "processeur.h"

void call(Processeur* proc, int len, int64* adresse){
    addFirstLL(proc->pileAppel, (void *) (proc->rip + len));
    proc->rip = *adresse;
}

void _jmp(Processeur* proc, int64* adresse){
    proc->rip = *adresse;
}

void _jne(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _je(Processeur* proc, int len, int64* adresse){
    if (proc->ZF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _ja(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF && !proc->CF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _jb(Processeur* proc, int len, int64* adresse){
    if (proc->CF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _jbe(Processeur* proc, int len, int64* adresse){
    if (proc->ZF || proc->CF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _jg(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF && proc->SF == proc->OF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _jle(Processeur* proc, int len, int64* adresse){
    if (proc->ZF || proc->SF != proc->OF) {
        _jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void _add(Processeur* proc, int64* destination, int64* masque){
    *destination += *masque;
    // pas fini
}

void _move(int64* dest, int64* source){
    *dest = *source;
}









