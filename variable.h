//
//  variable.h
//  desassembleur
//
//  Created by Hubert Godfroy on 01/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/*! 
 * @file variable.h
 */

#ifndef desassembleur_variable_h
#define desassembleur_variable_h
#include <stdint.h>
#include "macro.h"
#include "registre.h"
#include "memoire.h"

typedef struct _variable{
    uint8_t type;
    Registre* reg;
    Segment seg;
}Variable;



uint64_t getVarVal(Variable);

int getVarClassRec(Variable);

uint64_t setVarVal(Variable, uint64_t);

/*!
 * Donne la taille en bits de la variable.
 */

int getVarTaille(Variable);

void setVarClassRec(Variable, int classe);


#endif
