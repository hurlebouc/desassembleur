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

static int sf_aux(Variable var){
    
    if (getVarClassRec(var) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t n = getVarVal(var);
    int t = getVarTaille(var);
    if (getbits(n, t) == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int pf_aux(Variable var){
    
    if (getVarClassRec(var) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint8_t weakbits = getVarVal(var);
    int nbr_up = nbrup(weakbits);
    if (1 - (nbr_up % 2) == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int zf_aux(Variable a, Variable b){
    
    if (b.type == ukn_type) {
        if (getVarClassRec(a) == CLASSE_NON_DEFINIE) {
            return FLAG_NON_DEFINI;
        }
        if (getVarVal(a) == 0) {
            return FLAG_HAUT;
        } else {
            return FLAG_BAS;
        }
    }
    
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE ||
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    if (getVarVal(a) == getVarVal(b)) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}
/*!
 * @deprecated
 */
static int app_aux(int aux(const Variable, const Variable, const Variable), const Variable a, const Variable b, const Variable c){
    return aux(a,b,c);
}
/*!
 * @deprecated
 */
static Variable app_f(Variable f(Variable, Variable, Variable, Processeur*, int), Variable a, Variable b, Variable c, Processeur* proc, int lenInstr){
    return f(a,b,c,proc,lenInstr);
}

/*!
 * Cette fonction sert à formaliser l'utilisation les méthodes d'une 
 * instruction afin d'obliger leur utilisation dans un ordre pré-établi.
 */

Variable do_instr(Instruction* instr, Variable a, Variable b, Variable c, int lenInstr, Processeur* proc){
    
    if (instr->zf_aux == UNLOCKED) {
        _ZF = zf_aux(a, b);
    }
    
    int af = instr->af_aux(a, b,c);
    if (af != FLAG_UNMODIFIED) {
        _AF = af;
    }
    
    int cf = instr->cf_aux(a, b,c);
    if (cf != FLAG_UNMODIFIED) {
        _CF = cf;
    }
    
    int of = instr->of_aux(a, b,c);
    if (of != FLAG_UNMODIFIED) {
        _OF = of;
    }
    
    Variable _res = instr->f(a, b, c, proc, lenInstr);
    
    if (instr->sf_aux == UNLOCKED) {
        _SF = sf_aux(_res);
    }
    
    if (instr->pf_aux == UNLOCKED) {
        _PF = pf_aux(_res);
    }
    
    return _res;
}

Instruction* newInstruction(
                            int of(const Variable, 
                                   const Variable,
                                   const Variable),
                            int cf(const Variable, 
                                   const Variable,
                                   const Variable),
                            int af(const Variable, 
                                   const Variable,
                                   const Variable),
                            int zf,
                            int pf,
                            int sf, 
                            Variable f(Variable, 
                                        Variable, 
                                        Variable,
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

