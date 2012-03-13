//
//  registre.c
//  desassembleur
//
//  Created by Hubert Godfroy on 13/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "registre.h"

registre* newRegistreFeuille(int t, uint64_t v){
    registre* reg = malloc(sizeof(registre));
    reg->taille = t;
    reg->valeur = v;
    reg->filsh = NULL;
    reg->filsl = NULL;
    return reg;
}

registre* newRegistreFilsUnique(registre* fils){
    registre* reg = malloc(sizeof(registre));
    reg->taille = 2*fils->taille;
    reg->valeur = -1;
    reg->filsl = fils;
    reg->filsh = newRegistreFeuille(fils->taille, 0);
    return reg;
}

registre* newRegistreDeuxFils(registre* filsh, registre* filsl){
    registre* reg = malloc(sizeof(registre));
    reg->filsh = filsh;
    reg->filsl = filsl;
    reg->taille = filsh->taille + filsl->taille;
    reg->valeur = -1;
    return reg;
}

uint64_t getValeur(registre* reg){
    if (reg->filsl == NULL) {
        return reg->valeur;
    }
    return getValeur(reg->filsl) + getValeur(reg->filsh)*pow(2, reg->filsl->taille);
}

uint64_t setValeur(registre* reg, uint64_t n){
    uint64_t p = pow((long double) 2, reg->taille);
    uint64_t reste = n % p;
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