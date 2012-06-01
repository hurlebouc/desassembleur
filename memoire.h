//
//  memoire.h
//  desassembleur
//
//  Created by Hubert Godfroy on 31/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_memoire_h
#define desassembleur_memoire_h

#include <stdint.h>
#include "definitions.h"

typedef struct _case_mem{
    uint64_t virtualAddr;
    uint8_t val;
    uint8_t classe;
}case_mem;

typedef  struct _memoire {
    uint64_t size;
    case_mem** tabCorrespondance;
}Memoire;

Memoire* newMemoire(uint64_t tailleMax);
void terminateMemoire(Memoire* mem);

/**
 * Ne peut marcher que si la classe est définie.
 * Lecture en little-endian (le premier byte donne les 8 bits de poids les plus 
 * faibles et byte++ donne les suivants).
 */

uint64_t getVal(Memoire* mem, uint64_t virtualAddr, int taille);
int getMemClass(Memoire* mem, uint64_t virtualAddr, int taille);

void setVal(Memoire* mem, uint64_t virtualAddr, int taille, uint64_t val);

#endif
