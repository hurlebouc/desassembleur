//
//  memoire.c
//  desassembleur
//
//  Created by Hubert Godfroy on 31/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "memoire.h"

static case_mem* newCorr_mem(){
    case_mem* res = malloc(sizeof(case_mem));
    res->classe = CLASSE_NON_DEFINI;
    return res;
}

static void terminateCaseMem(case_mem* cas){
    free(cas);
}

static void setCaseVal(case_mem* corr, uint8_t val){
    corr->val = val;
    if (corr->classe == CLASSE_NON_DEFINI) {
        corr->classe = CLASSE_DEFINI;
    }
}

Memoire* newMemoire(uint64_t tailleMax){
    Memoire* res = malloc(sizeof(Memoire));
    res->taille = tailleMax;
    res->tabCorrespondance = malloc(sizeof(case_mem*)*tailleMax);
    return res;
}

void terminateMemoire(Memoire* mem){
    for (uint64_t i = 0; i<mem->taille; i++) {
        if (mem->tabCorrespondance[i] == NULL) {
            break;
        }
        terminateCaseMem(mem->tabCorrespondance[i]);
    }
    free(mem->tabCorrespondance);
    free(mem);
}

uint64_t getVal(Memoire* mem, uint64_t virtualAddr, int taille){
    
}








