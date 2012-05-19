//
//  liste_instr.c
//  desassembleur
//
//  Created by Hubert Godfroy on 24/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "liste_instr.h"
#include "_macro_Build.h"

/*--------------------------------------------------------------------------*/

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

/* ----------------------- AND -----------------------*/

static int of_and(const Registre* a, const Registre* b, const Registre* c){
    return 0;
}

static int cf_and(const Registre* a, const Registre* b, const Registre* c){
    return 0;
}

static int af_and(const Registre* a, const Registre* b, const Registre* c){
    return -1;
}

static Registre* f_and(Registre* destination, Registre* masque, Registre* c,Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
    uint64_t dest = getValeur(destination);
    uint64_t mask = getValeur(masque);
    dest &= mask;
    setValeur(destination, dest);
    return destination;
}

Instruction* init_and(){
    return newInstruction(of_and, cf_and, af_and, 1, 1, 1, f_and);
}

/* ----------------------- ADD -----------------------*/

static int of_add(const Registre* a, const Registre* b, const Registre* stub){
    uint64_t aa = getValeur(a);
    uint64_t bb = getValeur(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, a->taille);
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
        return 1;
    } else {
        return 0;
    }
}

static int cf_add(const Registre* a, const Registre* b, const Registre* stub){
    uint64_t aa = getValeur(a);
    uint64_t bb = getValeur(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, a->taille);
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
        return 1;
    } else {
        return 0;
    }
}

static int af_add(const Registre* a, const Registre* b, const Registre* stub){
    uint64_t aa = getValeur(a) % 8; // donne les 3 bits les plus faibles
    uint64_t bb = getValeur(b) % 8;
    
    return (aa + bb)/ 8;
}

static Registre* f_add(Registre* destination, Registre* masque, Registre* stub , Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
    uint64_t a = getValeur(destination);
    uint64_t b = getValeur(masque);
    uint64_t c = a+b;
    setValeur(destination, c);
    return destination;
}

Instruction* init_add(){
    return newInstruction(of_add, cf_add, af_add, 1, 1, 1, f_add);
}

/* ----------------------- MOV -----------------------*/
static int of_mov(const Registre* a, const Registre* b, const Registre* stub){
    return -1;
}

static int cf_mov(const Registre* a, const Registre* b, const Registre* stub){
    return -1;
}
static int af_mov(const Registre* a, const Registre* b, const Registre* stub){
    return -1;
}

static Registre* f_mov(Registre* gauche, Registre* droite, Registre* stub, Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
    uint64_t a = getValeur(gauche);
    setValeur(droite, a);
    return droite;
    /*
     * Que se passe t'il si les deux registres n'ont pas la même taille ?
     * Dans ce cas, comment se passe une extention d'une valeur (signé, non signé) ?
     * Si la destinantion est plus petites que la source et qu'il y a overflow, est-ce que le mov modife les registres de flags
     */
}

Instruction* init_mov(){
    return newInstruction(of_mov, cf_mov, af_mov, 0, 0, 0, f_mov);
}



/*----------------------------------------------------------------*/

static int f(int n){
    return n*n;
}

test* init_test(){
    test* t = newTest(f);
    return t;
}

