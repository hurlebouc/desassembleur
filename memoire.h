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
    SEG_NON_INITIALISE =    0x1,
    SEG_INDETERMINEE =      0x2,
    SEG_NON_PRESENT =       0x4,
    };

typedef struct _case_mem{
    uint64_t virtualAddr;
    uint8_t val;
    uint8_t classe;
}case_mem;

typedef  struct _memoire {
    uint64_t size;
    uint64_t sizeAllocatedMemory;
    case_mem** tabBytes;
}Memoire;

typedef struct _segment{
    Memoire* mem;
    uint64_t virtualAddr;
    uint8_t taille;
}Segment;

Memoire* newMemoire(uint64_t tailleMax);
void terminateMemoire(Memoire* mem);

Segment seg(Memoire* mem, uint64_t virtualAddr, int taille);

/* ---------------------------------------------------------------------- *
 *                       FONCTIONS ESSENTIELLES                           *
 * ---------------------------------------------------------------------- */

/**
 * Ne peut marcher que si la classe est définie.
 * Lecture en little-endian (le premier byte donne les 8 bits de poids les plus 
 * faibles et byte++ donne les suivants).
 */

uint64_t getSegVal(Segment seg);

/**
 * Cette fonction donne la classe de la valeur souhaité à partir de l'adresse 
 * virtualAddr sur une longueur taille.
 *
 * @param mem mémoire sur laquelle on travaille
 * @param virtualAddr adresse souhaitée dans le mémoire
 * @param taille longueur du mot lu
 * @return un tableau de taille 2 dont le premier élément est la classe, 
 *          le deuxième est toujours nul en cas de case déterminé
 */
int* getSegClass(Segment seg);

/**
 * Cette fonction assigne une dans la mémore à l'adresse virtualAddr sur une
 * taille donnée
 *
 * @param mem mémoire sur laquelle on travaille
 * @param virtualAddr adresse souhaitée dans la mémoire
 * @param taille longueur du mot dans lequel on souhaite écrire
 * @param val valeur à écrire
 * @return partie haute de val qui n'a pas été écrite par manque de place
 */
uint64_t setSegVal(Segment seg, uint64_t val);

void setSegClass(Segment, int classe);

/* ---------------------------------------------------------------------- *
 *                          FONCTIONS ANNEXES                             *
 * ---------------------------------------------------------------------- */

void copieMemVal(Memoire* dest, Memoire* src);
void cloneMem(Memoire* dest, Memoire* src);

void afficheMemoire(Memoire* mem);

#endif
