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
    res->classe = CLASSE_NON_DEFINIE;
    return res;
}

static void terminateCaseMem(case_mem* cas){
    free(cas);
}

static void setCaseVal(case_mem* corr, uint8_t val){
    corr->val = val;
    if (corr->classe == CLASSE_NON_DEFINIE) {
        corr->classe = CLASSE_DEFINI;
    }
}

Memoire* newMemoire(uint64_t tailleMax){
    if (tailleMax == UINT64_MAX) {
        printf("On ne peut crée un tableau de cette taille car il doit rester un élément (-1) ou %llu pour la gestion d'erreur.\n", UINT64_MAX);
        exit(EXIT_FAILURE);
    }
    Memoire* res = malloc(sizeof(Memoire));
    res->size = tailleMax;
    res->sizeAllocatedMemory = 0;
    res->tabCorrespondance = calloc(tailleMax, sizeof(case_mem*));
    return res;
}

void terminateMemoire(Memoire* mem){
    for (uint64_t i = 0; i<mem->sizeAllocatedMemory; i++) {
        terminateCaseMem(mem->tabCorrespondance[i]);
    }
    free(mem->tabCorrespondance);
    free(mem);
}
/**
 * Cette fonction cherche l'index de virtuAddr dans le tableau des 
 * correspondances (qui doit être trié par ordre croissant des virtualAddr).
 */
static uint64_t getByteIndex(Memoire* mem, uint64_t virtualAddr){
    uint64_t inf = 0;
    uint64_t sup = mem->sizeAllocatedMemory;
    while (sup-inf > 1) {
        uint64_t milieu = (sup + inf)/2;
        if (mem->tabCorrespondance[milieu]->virtualAddr == virtualAddr) {
            return milieu;
        }
        if (milieu > virtualAddr) {
            sup = milieu;
        } else {
            inf = milieu + 1;
        }
    }
    if (sup <= inf) { // == devrait suffir
        return -1;
    }
    return inf;
}

int* getSegClass(Memoire* mem, uint64_t virtualAddr, int taille){
    uint64_t i = getByteIndex(mem, virtualAddr);
    if (i==-1) {
        printf("l'élément %llu est introuvable\n", virtualAddr);
        exit(EXIT_FAILURE);
    }
    int* res = calloc(2, sizeof(int));
    res[0] = CLASSE_DEFINI;
    
    if (i + taille > mem->sizeAllocatedMemory) {
        res[0] = CLASSE_NON_DEFINIE;
        res[1] += SEG_NON_INITIALISE;
    }
    for (int j = 0; j < taille; j++) {
        if (virtualAddr + j != mem->tabCorrespondance[i+j]->virtualAddr) {
            res[0] = CLASSE_NON_DEFINIE;
            res[1] += SEG_NON_PRESENT;
        }
    }
    for (int j = 0; j < taille; j++) {
        if (mem->tabCorrespondance[i+j]->classe == CLASSE_NON_DEFINIE) {
            res[0] = CLASSE_NON_DEFINIE,
            res[1] += SEG_INDETERMINEE;
        }
    }
    return res;
}

uint64_t getSegVal(Memoire* mem, uint64_t virtualAddr, int taille){
    uint64_t i = getByteIndex(mem, virtualAddr);
    if (i==-1) {
        printf("l'élément %llu est introuvable\n", virtualAddr);
        exit(EXIT_FAILURE);
    }
    int* classe = getSegClass(mem, virtualAddr, taille);
    if (classe[0] == CLASSE_NON_DEFINIE) {
        printf("La valeur de la case %llu pour la taille %d est indéterminée\n", virtualAddr, taille);
        exit(EXIT_FAILURE);
    }
    free(classe);
    uint64_t res = 0;
    uint64_t p = 1;
    for (int j = 0; j<taille; j++) {
        case_mem* cas = mem->tabCorrespondance[i+j];
        res += cas->val * p;
        p = p*2;
    }
    return res;
}

static uint64_t insertCase(Memoire* mem, uint64_t virtualAddr){
    
}

static void shiftRight(Memoire* mem, uint64_t index){
    
}

static uint64_t initSegment(Memoire* mem, uint64_t virtualAddr, int taille){
    uint64_t i = getByteIndex(mem, virtualAddr);
    if (i == -1) {
        
        return ;
    }
    int* classe = getSegClass(mem, virtualAddr, taille);
    if (classe[0] == CLASSE_NON_DEFINIE) {
        
        return i;
    }
    return i;
}

uint64_t setSegVal(Memoire* mem, uint64_t virtualAddr, int taille, uint64_t val){
    uint64_t i = initSegment(mem, virtualAddr, taille);
    uint64_t p = 1;
    for (int j = 0; j < taille; j++) {
        mem->tabCorrespondance[i+j]->classe = CLASSE_DEFINI;
        mem->tabCorrespondance[i+j]->val = val % 0xff;
        val = val / 0xff;
        p = p * 0xff;
    }
    return val*p;
}








