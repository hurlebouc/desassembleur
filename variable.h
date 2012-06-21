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
#include "stdint.h"
#include "registre.h"
#include "memoire.h"

enum TypeVariable {
    ukn_type = 0,
    reg_type = 1,
    seg_type = 2,
    };

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
