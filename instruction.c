//
//  instruction.c
//  desassembleur
//
//  Created by Hubert Godfroy on 23/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "instruction.h"
#include "_macro_Build.h"

static int nbrup(uint8_t n){
    int res = 0;
    for (int i = 0; i<8; i++) {
        if (n%2 == 1) {
            res++;
        }
        n = n/2;
    }
    return res;
}

static int getbits(uint64_t n, int p){
    for (int i = 0; i<p-1; i++) {
        n=n/2;
    }
    return  n % 2;
}

static int sf_aux(Registre* reg){
    uint64_t n = getValeur(reg);
    int t = reg->taille;
    if (getbits(n, t) == 1) {
        return 1;
    } else {
        return 0;
    }
}

static int pf_aux(Registre* reg){
    uint8_t weakbits = getValeur(reg);
    int nbr_up = nbrup(weakbits);
    return 1 - (nbr_up % 2);
}

static int zf_aux(Registre* a, Registre* b){
    if (getValeur(a) == getValeur(b)) {
        return 1;
    } else {
        return 0;
    }
}

static int app_aux(int aux(const Registre*, const Registre*), const Registre*a, const Registre*b){
    return aux(a,b);
}

static Registre* app_f(Registre* f(Registre*, Registre*, Processeur*), Registre*a, Registre*b, Processeur* proc){
    return f(a,b,proc);
}

void do_instr(Instruction* instr, Registre* a, Registre* b,int lenInstr, Processeur* proc){
    incr(_RIP, lenInstr);
    
    if (instr->zf_aux) {
        _ZF = zf_aux(a, b);
    }
    
    if (app_aux(instr->af_aux, a, b) != -1) {
        _AF = app_aux(instr->af_aux, a, b);
    }
    if (app_aux(instr->cf_aux, a, b) != -1) {
        _CF = app_aux(instr->cf_aux, a, b);
    }
    if (app_aux(instr->of_aux, a, b) != -1) {
        _OF = app_aux(instr->of_aux, a, b);
    }
    
    Registre* _res = app_f(instr->f, a, b, proc); // modification de l'état du processeur (sauf IP et flags)
    
    if (instr->sf_aux) {
        _SF = sf_aux(_res);
    }
    if (instr->pf_aux) {
        _PF = pf_aux(_res);
    }
}

Instruction* newInstruction(void* of,void* cf,void* af,int zf,int pf,int sf, void*f){
    Instruction* res = malloc(sizeof(res));
    res->of_aux = of;
    res->cf_aux = cf;
    res->af_aux = af;
    res->zf_aux = zf;
    res->pf_aux = pf;
    res->sf_aux = sf;
    res->f = f;
    return res;
}

void terminateInstruction(Instruction* instr){
    free(instr);
}

/*---------------------------------------------------*/

static int app_test(int f(int), int a){
    return f(a);
}

int do_test(test* t, int n){
    return app_test(t->f, n);
}


test* newTest(void* f){
    test* res = malloc(sizeof(test));
    res->f = f;
    return res;
}

