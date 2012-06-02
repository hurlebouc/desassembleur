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
    if (var.type == reg_type) {
        return getRegVal(var.reg);
    } else {
        return getSegVal(var.seg);
    }
}

int getVarClass(Variable var){
    if (var.type == reg_type) {
        return getRegClass(var.reg);
    } else {
        return getSegClass(var.seg)[0];
    }
}

uint64_t setVarVal(Variable var, uint64_t val){
    if (var.type == reg_type) {
        return setRegVal(var.reg, val);
    } else {
        return setSegVal(var.seg, val);
    }
}

int getVarTaille(Variable var){
    if (var.type == reg_type) {
        return var.reg->taille;
    } else {
        return var.seg.taille;
    }
}