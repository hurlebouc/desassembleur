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

void jmp(Processeur* proc, int64* adresse){
    proc->rip = *adresse;
}

void jne(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void je(Processeur* proc, int len, int64* adresse){
    if (proc->ZF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void ja(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF && !proc->CF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void jb(Processeur* proc, int len, int64* adresse){
    if (proc->CF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void jbe(Processeur* proc, int len, int64* adresse){
    if (proc->ZF || proc->CF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void jg(Processeur* proc, int len, int64* adresse){
    if (!proc->ZF && proc->SF == proc->OF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}

void jle(Processeur* proc, int len, int64* adresse){
    if (proc->ZF || proc->SF != proc->OF) {
        jmp(proc, adresse);
    } else {
        proc->rip += len;
    }
}










