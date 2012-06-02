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
//    return 0;
    return FLAG_BAS;
}

static int cf_and(const Registre* a, const Registre* b, const Registre* c){
//    return 0;
    return FLAG_BAS;
}

static int af_and(const Registre* a, const Registre* b, const Registre* c){
//    return -1;
    return FLAG_UNMODIFIED;
}

static Registre* f_and(Registre* destination, Registre* masque, Registre* stub,Processeur* proc, int lenInstr){
    
    if (masque->classe == CLASSE_NON_DEFINIE) {
        destination->classe = CLASSE_NON_DEFINIE;
        return destination;
    }
    
    incr(_RIP, lenInstr);
    uint64_t dest = getRegVal(destination);
    uint64_t mask = getRegVal(masque);
    dest &= mask;
    setRegVal(destination, dest);
    return destination;
}

Instruction* init_and(){
//    return newInstruction(of_and, cf_and, af_and, 1, 1, 1, f_and);
    return newInstruction(of_and, cf_and, af_and, UNLOCKED, UNLOCKED, UNLOCKED, f_and);
}

/* ----------------------- ADD -----------------------*/

static int of_add(const Registre* a, const Registre* b, const Registre* stub){
    
    if (a->classe == CLASSE_NON_DEFINIE ||
        b->classe == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t aa = getRegVal(a);
    uint64_t bb = getRegVal(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, a->taille);
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
//        return 1;
        return FLAG_HAUT;
    } else {
//        return 0;
        return FLAG_BAS;
    }
}

static int cf_add(const Registre* a, const Registre* b, const Registre* stub){
    
    if (a->classe == CLASSE_NON_DEFINIE ||
        b->classe == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t aa = getRegVal(a);
    uint64_t bb = getRegVal(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, a->taille);
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
//        return 1;
        return FLAG_HAUT;
    } else {
//        return 0;
        return FLAG_BAS;
    }
}

static int af_add(const Registre* a, const Registre* b, const Registre* stub){
    
    if (a->classe == CLASSE_NON_DEFINIE ||
        b->classe == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t aa = getRegVal(a) % 8; // donne les 3 bits les plus faibles
    uint64_t bb = getRegVal(b) % 8;
    if ((aa + bb) / 8 == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
//    return (aa + bb) / 8;
}

static Registre* f_add(Registre* destination, Registre* masque, Registre* stub , Processeur* proc, int lenInstr){
    
    if (masque->classe == CLASSE_NON_DEFINIE) {
        destination->classe = CLASSE_NON_DEFINIE;
        return destination;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getRegVal(destination);
    uint64_t b = getRegVal(masque);
    uint64_t c = a+b;
    setRegVal(destination, c);
    return destination;
}

Instruction* init_add(){
//    return newInstruction(of_add, cf_add, af_add, 1, 1, 1, f_add);
    return newInstruction(of_add, cf_add, af_add, UNLOCKED, UNLOCKED, UNLOCKED, f_add);
}

/* ----------------------- MOV -----------------------*/
static int of_mov(const Registre* a, const Registre* b, const Registre* stub){
//    return -1;
    return FLAG_UNMODIFIED;
}

static int cf_mov(const Registre* a, const Registre* b, const Registre* stub){
//    return -1;
    return FLAG_UNMODIFIED;
}
static int af_mov(const Registre* a, const Registre* b, const Registre* stub){
//    return -1;
    return FLAG_UNMODIFIED;
}

static Registre* f_mov(Registre* gauche, Registre* droite, Registre* stub, Processeur* proc, int lenInstr){
    
    if (gauche->classe == CLASSE_NON_DEFINIE) {
        droite->classe = CLASSE_NON_DEFINIE;
        return droite;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getRegVal(gauche);
    setRegVal(droite, a);
    return droite;
    /*
     * Que se passe t'il si les deux registres n'ont pas la même taille ?
     * Dans ce cas, comment se passe une extention d'une valeur (signé, non signé) ?
     * Si la destinantion est plus petites que la source et qu'il y a overflow, est-ce que le mov modife les registres de flags
     */
}

Instruction* init_mov(){
//    return newInstruction(of_mov, cf_mov, af_mov, 0, 0, 0, f_mov);
    return newInstruction(of_mov, cf_mov, af_mov, LOCKED, LOCKED, LOCKED, f_mov);
}



/*----------------------------------------------------------------*/

static int f(int n){
    return n*n;
}

test* init_test(){
    test* t = newTest(f);
    return t;
}

