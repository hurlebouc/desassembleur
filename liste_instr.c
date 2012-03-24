//
//  liste_instr.c
//  desassembleur
//
//  Created by Hubert Godfroy on 24/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "liste_instr.h"

static int f(int n){
    return n*n;
}

test* init_t(){
    test* t = newTest(f);
    return t;
}

/* ----------------------- AND -----------------------*/

static int of_and(const Registre* a, const Registre* b){
    return 0;
}

static int cf_and(const Registre* a, const Registre* b){
    return 0;
}

static int af_and(const Registre* a, const Registre* b){
    return -1;
}

static Registre* f_and(Registre* destination, Registre* masque, Processeur* proc){
    uint64_t dest = getValeur(destination);
    uint64_t mask = getValeur(masque);
    dest &= mask;
    setValeur(destination, dest);
    return destination;
}

Instruction* init_and(){
    return newInstruction(of_and, cf_and, af_and, 1, 1, 1, f_and);
}