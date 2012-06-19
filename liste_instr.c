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

/*!
 * Donne le p-eme bit de n en partant du bit le plus faible (commence à 0)
 */
static int getbits(uint64_t n, int p){
    for (int i = 0; i<p; i++) {
        n=n/2;
    }
    return  n % 2;
}

/*!
 * Donne le nombre de bits sur lesquels est écrit n
 */
static int getSize(uint64_t n){
    int res = 0;
    while (n != 0) {
        n = n/2;
        res++;
    }
    return res;
}

/*!
 * Donne l'index du bit de poind le plus faible à 1 (commance à 0)
 */
static int getminbit(uint64_t n){
    if (n==0) {
        return -1;
    }
    int res = 0;
    while (n%2 == 0) {
        res++;
        n = n/2;
    }
    return res;
}

/* ----------------------- AND -----------------------*/

static int of_and(const Variable a, const Variable b, const Variable c){
//    return 0;
    return FLAG_BAS;
}

static int cf_and(const Variable a, const Variable b, const Variable c){
//    return 0;
    return FLAG_BAS;
}

static int af_and(const Variable a, const Variable b, const Variable c){
//    return -1;
    return FLAG_UNMODIFIED;
}

static Variable f_and(Variable destination, Variable masque, Variable stub,Processeur* proc, int lenInstr){
    
    if (getVarClassRec(masque) == CLASSE_NON_DEFINIE || 
        getVarClassRec(destination) == CLASSE_NON_DEFINIE) {
        setVarClassRec(destination, CLASSE_NON_DEFINIE);
        return destination;
    }
    
    incr(_RIP, lenInstr);
    uint64_t dest = getVarVal(destination);
    uint64_t mask = getVarVal(masque);
    dest &= mask;
    setVarVal(destination, dest);
    return destination;
}

Instruction* init_and(){
//    return newInstruction(of_and, cf_and, af_and, 1, 1, 1, f_and);
    return newInstruction(of_and, cf_and, af_and, UNLOCKED, UNLOCKED, UNLOCKED, f_and);
}

/* ----------------------- ADD -----------------------*/

static int of_add(const Variable a, const Variable b, const Variable stub){
    
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE ||
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t aa = getVarVal(a);
    uint64_t bb = getVarVal(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, getVarTaille(a));
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int cf_add(const Variable a, const Variable b, const Variable stub){
    
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE ||
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    
    uint64_t aa = getVarVal(a);
    uint64_t bb = getVarVal(b);
    uint64_t c = aa+bb;
    uint64_t p = pow(2, getVarTaille(a));
    if (p!= 0) {
        c = c % p;
    }
    if (c<aa) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int af_add(const Variable a, const Variable b, const Variable stub){
    
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE ||
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t aa = getVarVal(a) % 8; // donne les 3 bits les plus faibles
    uint64_t bb = getVarVal(b) % 8;
    if ((aa + bb) / 8 == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static Variable f_add(Variable destination, Variable masque, Variable stub , Processeur* proc, int lenInstr){
    
    if (getVarClassRec(masque) == CLASSE_NON_DEFINIE || 
        getVarClassRec(destination) == CLASSE_NON_DEFINIE) {
        setVarClassRec(destination, CLASSE_NON_DEFINIE);
        return destination;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getVarVal(destination);
    uint64_t b = getVarVal(masque);
    uint64_t c = a+b;
    setVarVal(destination, c);
    return destination;
}

Instruction* init_add(){
    return newInstruction(of_add, cf_add, af_add, UNLOCKED, UNLOCKED, UNLOCKED, f_add);
}

/* ----------------------- MOV -----------------------*/
static int of_mov(const Variable a, const Variable b, const Variable stub){
//    return -1;
    return FLAG_UNMODIFIED;
}

static int cf_mov(const Variable a, const Variable b, const Variable stub){
//    return -1;
    return FLAG_UNMODIFIED;
}
static int af_mov(const Variable a, const Variable b, const Variable stub){
//    return -1;
    return FLAG_UNMODIFIED;
}

static Variable f_mov(Variable gauche, Variable droite, Variable stub, Processeur* proc, int lenInstr){
    
    if (getVarClassRec(droite) == CLASSE_NON_DEFINIE) {
        setVarClassRec(gauche, CLASSE_NON_DEFINIE);
        return gauche;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getVarVal(droite);
    setVarVal(gauche, a);
    return gauche;
    /*
     * Que se passe t'il si les deux registres n'ont pas la même taille ?
     * Dans ce cas, comment se passe une extention d'une valeur (signé, non 
     * signé) ? Si la destinantion est plus petites que la source et qu'il y a 
     * overflow, est-ce que le mov modife les registres de flags
     */
}

Instruction* init_mov(){
//    return newInstruction(of_mov, cf_mov, af_mov, 0, 0, 0, f_mov);
    return newInstruction(of_mov, cf_mov, af_mov, LOCKED, LOCKED, LOCKED, f_mov);
}

/* ----------------------- SHL : shl eax 2 -----------------------*/ //d1

static int of_shl(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val > 1) {
        return FLAG_NON_DEFINI;
    }
    int bit = getbits(a_val, getVarTaille(a) - b_val - 1);
    int top_bit = getbits(a_val, getVarTaille(a) - 1);
    if (bit != top_bit) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int cf_shl(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val == 0) {
        return FLAG_UNMODIFIED;
    }
    int bit = getbits(a_val, getVarTaille(a) - b_val);
    if (bit == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}
static int af_shl(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val > getVarTaille(a) - getSize(a_val)) {
        return FLAG_NON_DEFINI;
    }
    return FLAG_UNMODIFIED;
}

static Variable f_shl(Variable gauche, Variable droite, Variable stub, Processeur* proc, int lenInstr){
    
    if (getVarClassRec(gauche) == CLASSE_NON_DEFINIE ||
        getVarClassRec(droite) == CLASSE_NON_DEFINIE) {
        setVarClassRec(droite, CLASSE_NON_DEFINIE);
        return droite;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getVarVal(gauche);
    for (int i = 0; i<getVarVal(droite); i++) {
        a = a*2;
    }
    setVarVal(gauche, a);
    return gauche;
}

Instruction* init_shl(){
    return newInstruction(of_shl, cf_shl, af_shl, UNLOCKED, UNLOCKED, UNLOCKED, f_shl);
}

/* ----------------------- SHR : shr eax 2 -----------------------*/

static int of_shr(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val > 1) {
        return FLAG_NON_DEFINI;
    }
    int top_bit = getbits(a_val, getVarTaille(a) - 1);
    if (top_bit == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}

static int cf_shr(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val == 0) {
        return FLAG_UNMODIFIED;
    }
    int bit = getbits(a_val, b_val-1);
    if (bit == 1) {
        return FLAG_HAUT;
    } else {
        return FLAG_BAS;
    }
}
static int af_shr(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    uint64_t a_val = getVarVal(a);
    uint64_t b_val = getVarVal(b);
    if (b_val > getVarTaille(a)) {
        return FLAG_NON_DEFINI;
    }
    if (b_val > getminbit(a_val)) {
        return FLAG_NON_DEFINI;
    }
    return FLAG_UNMODIFIED;
}

static Variable f_shr(Variable gauche, Variable droite, Variable stub, Processeur* proc, int lenInstr){
    
    if (getVarClassRec(gauche) == CLASSE_NON_DEFINIE ||
        getVarClassRec(droite) == CLASSE_NON_DEFINIE) {
        setVarClassRec(droite, CLASSE_NON_DEFINIE);
        return droite;
    }
    
    incr(_RIP, lenInstr);
    uint64_t a = getVarVal(gauche);
    for (int i = 0; i<getVarVal(droite); i++) {
        a = a/2;
    }
    setVarVal(gauche, a);
    return gauche;
}

Instruction* init_shr(){
    return newInstruction(of_shr, cf_shr, af_shr, UNLOCKED, UNLOCKED, UNLOCKED, f_shr);
}

/* ----------------------- PUSH -----------------------*/

static int of_push(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static int cf_push(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}
static int af_push(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_push(Variable a, Variable stub1, Variable stub2, Processeur* proc, int lenInstr){
    pushStack(_STACK, a, _RSP);
    incr(_RIP, lenInstr);
    return a;
}

Instruction* init_push(){
    return newInstruction(of_push, cf_push, af_push, LOCKED, LOCKED, LOCKED, f_push);
}

/* ----------------------- CMP -----------------------*/

/* NON SIGNE */

static int of_cmp(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static int cf_cmp(const Variable a, const Variable b, const Variable stub){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE || 
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        return FLAG_NON_DEFINI;
    }
    if (getVarVal(a) <= getVarVal(b)) {
        return FLAG_BAS;
    }
    return FLAG_HAUT;
}
static int af_cmp(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_cmp(Variable a, Variable stub1, Variable stub2, Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
    return a;
}

Instruction* init_cmp(){
    return newInstruction(of_cmp, cf_cmp, af_cmp, UNLOCKED, UNLOCKED, UNLOCKED, f_cmp);
}

/* ----------------------- JE -----------------------*/

static int of_je(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}
static int cf_je(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}
static int af_je(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_je(Variable a, Variable stub1, Variable stub2, Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
}

Instruction* init_je(){
    return newInstruction(of_je, cf_je, af_je, LOCKED, LOCKED, LOCKED, f_je);
}

/* ----------------------- IMUL -----------------------*/

static int of_imul(const Variable a, const Variable b, const Variable stub){
    return FLAG_BAS; // resultats simplifié 
    // levé ssi le bit de poids forts est dans la moitiée supérieure du registre final. (de même pour cf)
}
static int cf_imul(const Variable a, const Variable b, const Variable stub){
    return FLAG_BAS;
}
static int af_imul(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_imul(Variable a, Variable b, Variable c, Processeur* proc, int lenInstr){
    incr(_RIP, lenInstr);
    if (b.type == 0) {
        if (getVarClassRec(a) == CLASSE_NON_DEFINIE) {
            return a;
        }
        int taille = getVarTaille(a);
        uint64_t val = getVarVal(a);
        uint64_t rax = getRegVal(_RAX);
        if (taille != 64) {
            rax = rax % ((int) pow(2, taille));
        }
        uint64_t res = val*rax;
        setVarVal(a, res);
        return a;
    }
    if (c.type == 0) {
        if (getVarClassRec(a) == CLASSE_NON_DEFINIE ||
            getVarClassRec(b) == CLASSE_NON_DEFINIE) {
            return a;
        }
        uint64_t vala = getVarVal(a);
        uint64_t valb = getVarVal(b);
        uint64_t res = vala * valb;
        setVarVal(a, res);
        return a;
    }
    if (getVarClassRec(c) == CLASSE_NON_DEFINIE ||
        getVarClassRec(b) == CLASSE_NON_DEFINIE) {
        setVarClassRec(a, CLASSE_NON_DEFINIE);
        return a;
    }
    uint64_t valc = getVarVal(c);
    uint64_t valb = getVarVal(b);
    uint64_t res = valb * valc;
    setVarVal(a, res);
    return a;
}

Instruction* init_imul(){
    return newInstruction(of_imul, cf_imul, af_imul, LOCKED, LOCKED, LOCKED, f_imul);
}

/* ----------------------- SUB -----------------------*/

static int of_sub(const Variable a, const Variable b, const Variable stub){
    return FLAG_BAS; 
}
static int cf_sub(const Variable a, const Variable b, const Variable stub){
    return FLAG_BAS;
}
static int af_sub(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_sub(Variable a, Variable b, Variable c, Processeur* proc, int lenInstr){
    if (getVarClassRec(b) == CLASSE_NON_DEFINIE ||
        getVarClassRec(a) == CLASSE_NON_DEFINIE) {
        setVarClassRec(a, CLASSE_NON_DEFINIE);
        return a;
    }
    incr(_RIP, lenInstr);
    uint64_t vala = getVarVal(a);
    uint64_t valb = getVarVal(b);
    uint64_t res = vala - valb;
    setVarVal(a, res);
    return a;
}

Instruction* init_sub(){
    return newInstruction(of_sub, cf_sub, af_sub, UNLOCKED, UNLOCKED, UNLOCKED, f_sub);
}

/* ----------------------- INC -----------------------*/

static int of_inc(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED; 
}
static int cf_inc(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED; //vrai
}
static int af_inc(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED; //faux
}

static Variable f_inc(Variable a, Variable b, Variable c, Processeur* proc, int lenInstr){
    if (getVarClassRec(a) == CLASSE_NON_DEFINIE) {
        return a;
    }
    incr(_RIP, lenInstr);
    uint64_t vala = getVarVal(a);
    vala++;
    setVarVal(a, vala);
    return a;
}

Instruction* init_inc(){
    return newInstruction(of_inc, cf_inc, af_inc, UNLOCKED, UNLOCKED, UNLOCKED, f_inc);
}

/* ----------------------- POP -----------------------*/

static int of_pop(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static int cf_pop(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}
static int af_pop(const Variable a, const Variable b, const Variable stub){
    return FLAG_UNMODIFIED;
}

static Variable f_pop(Variable a, Variable stub1, Variable stub2, Processeur* proc, int lenInstr){
    popStack(_STACK, a, _RSP);
    incr(_RIP, lenInstr);
    return a;
}

Instruction* init_pop(){
    return newInstruction(of_pop, cf_pop, af_pop, LOCKED, LOCKED, LOCKED, f_pop);
}

/*----------------------------------------------------------------*/

static int f(int n){
    return n*n;
}

test* init_test(){
    test* t = newTest(f);
    return t;
}

