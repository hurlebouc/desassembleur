//
//  registre.c
//  desassembleur
//
//  Created by Hubert Godfroy on 13/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "registre.h"

Registre* newRegistreFeuille(int t/*, uint64_t v*/){
    Registre* reg = malloc(sizeof(Registre));
    reg->taille = t;
    reg->valeur = 0;
    reg->filsh = NULL;
    reg->filsl = NULL;
//    reg->classe = REGISTRE_DEFINI;
    reg->classe = CLASSE_NON_DEFINIE; 
    return reg;
}

Registre* newRegistreFilsUnique(Registre* fils){
    Registre* reg = malloc(sizeof(Registre));
    reg->taille = 2*fils->taille;
    reg->valeur = -1;
    reg->filsl = fils;
    reg->filsh = newRegistreFeuille(fils->taille/*, 0*/);
//    reg->classe = REGISTRE_DEFINI;
    reg->classe = CLASSE_NON_DEFINIE;
    return reg;
}

Registre* newRegistreDeuxFils(Registre* filsh, Registre* filsl){
    Registre* reg = malloc(sizeof(Registre));
    reg->filsh = filsh;
    reg->filsl = filsl;
    reg->taille = filsh->taille + filsl->taille;
    reg->valeur = -1;
//    reg->classe = REGISTRE_DEFINI;
    reg->classe = CLASSE_NON_DEFINIE;
    return reg;
}

void terminateRegistre(Registre* reg){
    if (reg->filsh != NULL) {
        terminateRegistre(reg->filsl);
        terminateRegistre(reg->filsh);
    }
    free(reg);
}

uint64_t getRegVal(const Registre* reg){
    if (reg->filsl == NULL) {
        return reg->valeur;
    }
    return getRegVal(reg->filsl) + getRegVal(reg->filsh)*pow(2, reg->filsl->taille);
}

uint64_t setRegVal(Registre* reg, uint64_t n){
    if (reg->classe == CLASSE_NON_DEFINIE) {
        reg->classe = CLASSE_DEFINI;
    }
    uint64_t p = pow(2, reg->taille);
    
    uint64_t reste;
    if (p==0) {
        reste = n;
    } else {
        reste = n % p;
    }
    //this->valeur = reste;
    if (reg->filsl != NULL) {
        uint64_t pl = pow(2, reg->filsl->taille);
        uint64_t restel = reste % pl;
        uint64_t quotienth = reste / pl;
        setRegVal(reg->filsl,restel);
        setRegVal(reg->filsh,quotienth);
    } else {
        reg->valeur = reste;
    }
    return n - reste; // différent de 0 lorsque ne peut pas être stocké dans le registre
}

void setRegClass(Registre* reg, int classe){
    if (reg == NULL) {
        return;
    }
    reg->classe = classe;
    setRegClass(reg->filsh, classe);
    setRegClass(reg->filsl, classe);
}

int getRegClass(Registre* reg){
    if (reg == NULL) {
        return CLASSE_DEFINI;
    }
    int n = getRegClass(reg->filsh)*getRegClass(reg->filsl)*reg->classe;
    if (n == 0) {
        return 0;
    } else {
        return reg->classe;
    }
}

void copieRegVal(Registre* dest, Registre* src){
    setRegVal(dest, getRegVal(src));
}

void incr(Registre* reg, int n){
    setRegVal(reg, getRegVal(reg) + n);
}