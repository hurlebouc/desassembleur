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

enum casIndeterminisationMemoire {
    CASE_NON_INITIALISE,
    CASE_INDETERMINEE,
    };

typedef struct _case_mem{
    uint64_t virtualAddr;
    uint8_t val;
    uint8_t classe;
}case_mem;

typedef  struct _memoire {
    uint64_t size;
    uint64_t sizeAllocatedMemory;
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

/**
 * Cette fonction donne la classe de la valeur souhaité à partir de l'adresse 
 * virtualAddr sur une longueur taille. On supposera que l'adresse existe dans 
 * la mémoire.
 *
 * @param mem mémoire sur laquelle on travail
 * @param virtualAddr adresse souhaité dans le mémoire
 * @param taille longueur du mot lu
 * @return un tableau de taille 2 dont le premier élément est la classe, 
 *          le deuxième est toujours nul en cas de case déterminé
 */
int* getMemClass(Memoire* mem, uint64_t virtualAddr, int taille);

void setVal(Memoire* mem, uint64_t virtualAddr, int taille, uint64_t val);

#endif
