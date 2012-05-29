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
/**
 * @deprecated
 */
static int app_aux(int aux(const Registre*, const Registre*, const Registre*), const Registre*a, const Registre*b, const Registre*c){
    return aux(a,b,c);
}
/**
 * @deprecated
 */
static Registre* app_f(Registre* f(Registre*, Registre*, Registre*, Processeur*, int), Registre*a, Registre*b, Registre*c, Processeur* proc, int lenInstr){
    return f(a,b,c,proc,lenInstr);
}

/**
 * Cette fonction sert à formaliser l'utilisation les méthodes d'une 
 * instruction afin d'obliger leur utilisation dans un ordre pré-établi.
 */

Registre* do_instr(Instruction* instr, Registre* a, Registre* b, Registre* c, int lenInstr, Processeur* proc){
    
    if (instr->zf_aux) {
        _ZF = zf_aux(a, b);
    }
    
    if (instr->af_aux(a, b,c) != -1) {
        _AF = app_aux(instr->af_aux, a, b, c);
    }
    if (instr->cf_aux(a, b,c) != -1) {
        _CF = app_aux(instr->cf_aux, a, b, c);
    }
    if (instr->of_aux(a, b,c) != -1) {
        _OF = app_aux(instr->of_aux, a, b, c);
    }
    
    Registre* _res = instr->f(a, b, c, proc, lenInstr);
    
    if (instr->sf_aux) {
        _SF = sf_aux(_res);
    }
    if (instr->pf_aux) {
        _PF = pf_aux(_res);
    }
    return _res;
}

Instruction* newInstruction(
                            int of(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int cf(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int af(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int zf,
                            int pf,
                            int sf, 
                            Registre* f(Registre*, 
                                        Registre*, 
                                        Registre*,
                                        Processeur*, int)
                            ){
    Instruction* res = malloc(sizeof(Instruction));
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
    return t->f(n);
}


test* newTest(int f(int)){
    test* res = malloc(sizeof(test));
    res->f = f;
    return res;
}

