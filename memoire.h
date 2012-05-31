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
#include "LinkedList.h"
#define TAILLE_MEMOIRE 0x1000

typedef struct _corr_mem{
    uint64_t virtualAddr;
    uint8_t val;
}corr_mem;

typedef  struct _memoire {
    uint64_t taille;
    corr_mem* tabCorrespondance[];
}Memoire;

Memoire* newMemoire(uint64_t tailleMax);



#endif
