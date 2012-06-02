//
//  variable.h
//  desassembleur
//
//  Created by Hubert Godfroy on 01/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_variable_h
#define desassembleur_variable_h
#include "stdint.h"
#include "registre.h"
#include "memoire.h"

enum TypeVariable {
    reg_type,
    mem_type,
    };

typedef struct _variable{
    uint8_t type;
    Registre* reg;
    Segment seg;
}Variable;



uint64_t getVarVal(Variable var);

#endif
