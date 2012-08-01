//
//  variable.c
//  desassembleur
//
//  Created by Hubert Godfroy on 01/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include "variable.h"

uint64_t getVarVal(Variable var){
    switch (var.type) {
        case reg_type:
            return getRegVal(var.reg);
            break;
        case seg_type:
            return getSegVal(var.seg);
            break;
        default:
            printf("getVarVal : le type de la variable est inconnu.\n");
            exit(EXIT_FAILURE);
            break;
    }
}

int getVarClassRec(Variable var){
    switch (var.type) {
        case reg_type:
            return getRegClassRec(var.reg);
            break;
        case seg_type:
            return getSegClassRec(var.seg)[0];
            break;
        default:
            printf("getVarClassRec : le type de la variable est inconnu.\n");
            exit(EXIT_FAILURE);
            break;
    }
}

uint64_t setVarVal(Variable var, uint64_t val){
    switch (var.type) {
        case reg_type:
            return setRegVal(var.reg, val);
            break;
        case seg_type:
            return setSegVal(var.seg, val);
            break;
        default:
            printf("setVarVal : le type de la variable est inconnu.\n");
            exit(EXIT_FAILURE);
            break;
    }
}

int getVarTaille(Variable var){
    switch (var.type) {
        case reg_type:
            return var.reg->taille;
            break;
        case seg_type:
            return var.seg.taille * 8;
            break;
        default:
            printf("getVarTaille : le type de la variable est inconnu.\n");
            exit(EXIT_FAILURE);
            break;
    }
}

void setVarClassRec(Variable var, int classe){
    switch (var.type) {
        case reg_type:
            setRegClassRec(var.reg, classe);
            break;
        case seg_type:
            setSegClassRec(var.seg, classe);
            break;
        default:
            printf("getVarTaille : le type de la variable est inconnu.\n");
            exit(EXIT_FAILURE);
            break;
    }
}