//
//  memoire.h
//  desassembleur
//
//  Created by Hubert Godfroy on 31/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/*!
 * @file memoire.h
 */

#ifndef desassembleur_memoire_h
#define desassembleur_memoire_h

#include <stdint.h>
#include "definitions.h"

enum casIndeterminisationMemoire {
    SEG_NON_INITIALISE =    0x1,
    SEG_INDETERMINEE =      0x2,
    SEG_NON_PRESENT =       0x4,
    };

typedef struct _byte{
    uint64_t virtualAddr;
    uint8_t val;
    uint8_t classe;
}Byte;

typedef  struct _memoire {
    uint64_t size;
    uint64_t sizeAllocatedMemory;
    Byte** tabBytes;
}Memoire;

typedef struct _segment{
    Memoire* mem;
    uint64_t virtualAddr;
    uint8_t taille; // en octet
}Segment;

Memoire* newMemoire(uint64_t tailleMax);
void terminateMemoire(Memoire* mem);

Segment seg(Memoire* mem, uint64_t virtualAddr, int taille);

/* ---------------------------------------------------------------------- *
 *                       FONCTIONS ESSENTIELLES                           *
 * ---------------------------------------------------------------------- */

/*!
 * Ne peut marcher que si la classe est définie.
 * Lecture en little-endian (le premier byte donne les 8 bits de poids les plus 
 * faibles et byte++ donne les suivants).
 */

uint64_t getSegVal(Segment seg);

/*!
 * Cette fonction donne la classe du segment souhaité sur une longueur donnée.
 *
 * @param seg segment de la mémoire dont on veut connaitre la classe
 * @return tableau de taille 2 dont le premier élément est la classe, 
 *          le deuxième est toujours nul en cas de byte déterminé
 */
int* getSegClassRec(Segment seg);

/*!
 * Cette fonction assigne une dans la mémore à l'adresse virtualAddr sur une
 * taille donnée
 *
 * @param seg segment de la mémoire sur lequel on souhaite écrire
 * @param val valeur à écrire
 * @return partie haute de val qui n'a pas été écrite par manque de place
 */
uint64_t setSegVal(Segment seg, uint64_t val);

/*!
 * 
 */
void setSegClassRec(Segment, int classe);

/* ---------------------------------------------------------------------- *
 *                          FONCTIONS ANNEXES                             *
 * ---------------------------------------------------------------------- */

void copieMemVal(Memoire* dest, Memoire* src);
void cloneMem(Memoire* dest, Memoire* src);
Byte* getByte(const Memoire*, uint64_t virtualAddr);
uint64_t getVirtualAddr(Byte*);
int getByteClass(Byte*);
uint8_t getByteVal(Byte*);
void setByteClass(Byte*, int classe);
void setByteVal(Byte*, uint8_t val);

void afficheMemoire(Memoire* mem);

#endif
