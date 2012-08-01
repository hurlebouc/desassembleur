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

static Byte* newCase_mem(){
    Byte* res = malloc(sizeof(Byte));
    res->classe = CLASSE_NON_DEFINIE;
    res->val = 0;
    res->virtualAddr = 0;
    return res;
}

static void terminateCaseMem(Byte* cas){
    free(cas);
}

static void setCaseVal(Byte* corr, uint8_t val){
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
    res->tabBytes = calloc(tailleMax, sizeof(Byte*));
    return res;
}

void terminateMemoire(Memoire* mem){
    for (uint64_t i = 0; i<mem->sizeAllocatedMemory; i++) {
        terminateCaseMem(mem->tabBytes[i]);
    }
    free(mem->tabBytes);
    free(mem);
}

Segment seg(Memoire* mem, uint64_t virtualAddr, int taille){
    Segment seg;
    seg.mem = mem;
    seg.taille = taille;
    seg.virtualAddr = virtualAddr;
    return seg;
}

Segment* newSegment(Memoire* mem, uint64_t virtualAddr, int taille){
    Segment* seg = malloc(sizeof(Segment));
    seg->mem = mem;
    seg->taille = taille;
    seg->virtualAddr = virtualAddr;
    return seg;
}

/*!
 * Cette fonction cherche l'index de virtuAddr dans le tableau des 
 * correspondances (qui doit être trié par ordre croissant des virtualAddr).
 */
static uint64_t getByteIndex(const Memoire* mem, uint64_t virtualAddr){
    uint64_t inf = 0;
    uint64_t sup = mem->sizeAllocatedMemory;
    while (sup-inf > 0) {
        uint64_t milieu = (sup + inf)/2;
        Byte* byte = mem->tabBytes[milieu];
        if (byte->virtualAddr == virtualAddr) {
            return milieu;
        }
        if (byte->virtualAddr > virtualAddr) {
            sup = milieu;
        } else {
            inf = milieu + 1;
        }
    }
    return -1;
}

static uint8_t testDrapeau(int typeErreur, int erreur){
    return (erreur & typeErreur) / typeErreur;
}

int* getSegClassRec(Segment seg){
    const Memoire* mem = seg.mem;
    const uint64_t virtualAddr = seg.virtualAddr;
    int taille = seg.taille;
    
    int* res = calloc(2, sizeof(int));
    res[0] = CLASSE_DEFINI;
    
    uint64_t i = getByteIndex(mem, virtualAddr);
    if (i==-1) {
        res[0] = CLASSE_NON_DEFINIE;
        res[1] = SEG_NON_PRESENT;
        return res;
//        printf("l'élément 0x%llx est introuvable\n", virtualAddr);
//        exit(EXIT_FAILURE);
    }
    
    for (int j = 0; j < taille; j++) {
        if (mem->tabBytes[i+j]->classe == CLASSE_NON_DEFINIE) {
            res[0] = CLASSE_NON_DEFINIE;
            if (!testDrapeau(SEG_INDETERMINEE, res[1])) {
                res[1] += SEG_INDETERMINEE;
            }
            res[1] += SEG_INDETERMINEE;
        }
        if (virtualAddr + j != mem->tabBytes[i+j]->virtualAddr) {
            res[0] = CLASSE_NON_DEFINIE;
            if (!testDrapeau(SEG_NON_PRESENT, res[1])) {
                res[1] += SEG_NON_PRESENT;
            }
        }
    }
    return res;
}

uint64_t getSegVal(Segment seg){
    const Memoire* mem = seg.mem;
    const uint64_t virtualAddr = seg.virtualAddr;
    const int taille = seg.taille;
    uint64_t i = getByteIndex(mem, virtualAddr);
    if (i==-1) {
        printf("l'élément %llu est introuvable\n", virtualAddr);
        exit(EXIT_FAILURE);
    }
    int* classe = getSegClassRec(seg);
    if (classe[0] == CLASSE_NON_DEFINIE) {
        printf("La valeur de la case 0x%llx pour la taille %d est indéterminée\n", virtualAddr, taille);
        exit(EXIT_FAILURE);
    }
    free(classe);
    uint64_t res = 0;
    uint64_t p = 1;
    for (int j = 0; j<taille; j++) {
        Byte* cas = mem->tabBytes[i+j];
        res += cas->val * p;
        p = p * 256;
    }
    return res;
}

static void shiftRight(Memoire* mem, uint64_t index){
    if (mem->size == mem->sizeAllocatedMemory) {
        printf("La mémoire alouée est trop petit pour entrer une nouvelle valeur\n");
        printf("(TODO faire de l'allocation dynamique)\n");
        exit(EXIT_FAILURE);
    }
    if (index > mem->sizeAllocatedMemory) {
        printf("l'index est hors de la mémoire et n'est pas à sa frontière\n");
        exit(EXIT_FAILURE);
    }
    for (uint64_t i = mem->sizeAllocatedMemory; i > index; i--) {
        mem->tabBytes[i] = mem->tabBytes[i-1];
    }
    mem->tabBytes[index] = newCase_mem();
    mem->sizeAllocatedMemory++;
}

static uint64_t insertCase(Memoire* mem, uint64_t virtualAddr){
    uint64_t inf = 0;
    uint64_t sup = mem->sizeAllocatedMemory;
    while (sup-inf > 0) {
        uint64_t milieu = (sup + inf)/2;
        Byte* byte = mem->tabBytes[milieu];
        if (byte->virtualAddr == virtualAddr) {
            return milieu;
        }
        if (byte->virtualAddr > virtualAddr) {
            sup = milieu;
        } else {
            inf = milieu + 1;
        }
    }
    shiftRight(mem, inf); // inf == sup
    mem->tabBytes[inf]->virtualAddr = virtualAddr;
    return inf;
}

static uint64_t initCase(Memoire* mem, uint64_t virtualAddr){
    return insertCase(mem, virtualAddr);
}

static uint64_t initSegment(Memoire* mem, uint64_t virtualAddr, int taille){
    uint64_t i = initCase(mem, virtualAddr);
    for (int j = 0; j<taille; j++) {
        Byte* byte = mem->tabBytes[i+j];
        if (byte == NULL || byte->virtualAddr != virtualAddr + j) {
            shiftRight(mem, i+j);
            mem->tabBytes[i+j]->virtualAddr = virtualAddr + j;
        }
    }
    return i;
}

uint64_t setSegVal(Segment seg, uint64_t val){
    Memoire* mem = seg.mem;
    const uint64_t virtualAddr = seg.virtualAddr;
    const int taille = seg.taille;
    uint64_t i = initSegment(mem, virtualAddr, taille);
    uint64_t p = 1;
    for (int j = 0; j < taille; j++) {
        mem->tabBytes[i+j]->classe = CLASSE_DEFINI;
        mem->tabBytes[i+j]->val = val % 0x100;
        val = val / 0x100;
        p = p * 0x100;
    }
    return val*p;
}

void setSegClassRec(Segment seg, int classe){
    Memoire* mem = seg.mem;
    const uint64_t virtualAddr = seg.virtualAddr;
    const int taille = seg.taille;
    uint64_t i = initSegment(mem, virtualAddr, taille);
    for (int j = 0; j<taille; j++) {
        mem->tabBytes[i+j]->classe = classe;
    }
}

void afficheMemoire(Memoire* mem){
    printf("[");
    for (uint64_t i = 0; i<mem->sizeAllocatedMemory; i++) {
        printf("[0x%llx, %x], ", mem->tabBytes[i]->virtualAddr,
               mem->tabBytes[i]->val);
    }
    printf("]\n");
}

/* ---------------------------------------------------------------------- *
 *                          FONCTIONS ANNEXES                             *
 * ---------------------------------------------------------------------- */

static void copieCaseVal(Byte* dest, Byte* src){
    if (dest == NULL || src == NULL) {
        printf("Erreur : aucun des paramètres ne doit être NULL");
        exit(EXIT_FAILURE);
    }
    dest->classe = src->classe;
    dest->val = src->val;
    dest->virtualAddr = src->virtualAddr;
}

void copieMemVal(Memoire* dest, Memoire* src){
    if (dest->size < src->sizeAllocatedMemory) {
        printf("Erreur : la taille maximale de la destination est insuffisante pour contenir la mémoire alloué de la source.\n");
        printf("Utilisez cloneMem() pour faire une copie conforme de la source.\n");
        exit(EXIT_FAILURE);
    }
    uint64_t max;
    uint64_t min;
    if (dest->sizeAllocatedMemory < src->sizeAllocatedMemory) {
        max = src->sizeAllocatedMemory;
        min = dest->sizeAllocatedMemory;
    } else {
        max = dest->sizeAllocatedMemory;
        min = src->sizeAllocatedMemory;
    }
    for (uint64_t i = 0; i < min; i++) {
        copieCaseVal(dest->tabBytes[i], src->tabBytes[i]);
    }
    
    for (uint64_t i = min; i<max;i++) {
        if (i >= src->sizeAllocatedMemory) {
            terminateCaseMem(dest->tabBytes[i]);
            dest->tabBytes[i] = NULL;
        } else {
            dest->tabBytes[i] = newCase_mem();
            copieCaseVal(dest->tabBytes[i], src->tabBytes[i]);
        }
    }
    dest->sizeAllocatedMemory = src->sizeAllocatedMemory;
}

void cloneMem(Memoire* dest, Memoire* src){
    if (dest->size == src->size) {
        copieMemVal(dest, src);
        return;
    }
    dest->size = src->size;
    for (uint64_t i = 0; i<dest->sizeAllocatedMemory; i++) {
        terminateCaseMem(dest->tabBytes[i]);
    }
    free(dest->tabBytes);
    dest->tabBytes = calloc(src->size, sizeof(Byte*));
    for (uint64_t i = 0; i<src->sizeAllocatedMemory; i++) {
        dest->tabBytes[i] = newCase_mem();
        copieCaseVal(dest->tabBytes[i], src->tabBytes[i]);
    }
    dest->sizeAllocatedMemory = src->sizeAllocatedMemory;
}

Byte* getByte(const Memoire* mem, uint64_t virtualAddr){
    uint64_t i = getByteIndex(mem, virtualAddr);
    return mem->tabBytes[i];
}

uint64_t getVirtualAddr(Byte* byte){
    return byte->virtualAddr;
}

int getByteClass(Byte* byte){
    if (byte == NULL) {
        return CLASSE_NON_DEFINIE;
    }
    return byte->classe;
}

uint8_t getByteVal(Byte* byte){
    return byte->val;
}

void setByteClass(Byte* byte, int classe){
    byte->classe = classe;
}

void setByteVal(Byte* byte, uint8_t val){
    byte->val = val;
}



