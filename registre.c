//
//  registre.c
//  desassembleur
//
//  Created by Hubert Godfroy on 13/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "registre.h"

Registre* newRegistreFeuille(int t, uint64_t v){
    Registre* reg = malloc(sizeof(Registre));
    reg->taille = t;
    reg->valeur = v;
    reg->filsh = NULL;
    reg->filsl = NULL;
    reg->classe = VALEUR_DEFINIE; 
    return reg;
}

Registre* newRegistreFilsUnique(Registre* fils){
    Registre* reg = malloc(sizeof(Registre));
    reg->taille = 2*fils->taille;
    reg->valeur = -1;
    reg->filsl = fils;
    reg->filsh = newRegistreFeuille(fils->taille, 0);
    reg->classe = VALEUR_DEFINIE; 
    return reg;
}

Registre* newRegistreDeuxFils(Registre* filsh, Registre* filsl){
    Registre* reg = malloc(sizeof(Registre));
    reg->filsh = filsh;
    reg->filsl = filsl;
    reg->taille = filsh->taille + filsl->taille;
    reg->valeur = -1;
    reg->classe = VALEUR_DEFINIE; 
    return reg;
}

void terminateRegistre(Registre* reg){
    if (reg->filsh != NULL) {
        terminateRegistre(reg->filsl);
        terminateRegistre(reg->filsh);
    }
    free(reg);
}

uint64_t getValeur(const Registre* reg){
    if (reg->filsl == NULL) {
        return reg->valeur;
    }
    return getValeur(reg->filsl) + getValeur(reg->filsh)*pow(2, reg->filsl->taille);
}

uint64_t setValeur(Registre* reg, uint64_t n){
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
        setValeur(reg->filsl,restel);
        setValeur(reg->filsh,quotienth);
    } else {
        reg->valeur = reste;
    }
    return n - reste; // différent de 0 lorsque ne peut pas être stocké dans le registre
}

int getClasse(Registre* reg){
    if (reg == NULL) {
        return VALEUR_DEFINIE;
    }
    int n = getClasse(reg->filsh)*getClasse(reg->filsl)*reg->classe;
    if (n == 0) {
        return 0;
    } else {
        return reg->classe;
    }
}

void copieVal(Registre* dest, Registre* src){
    setValeur(dest, getValeur(src));
}

void incr(Registre* reg, int n){
    setValeur(reg, getValeur(reg) + n);
}