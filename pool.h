//
//  pool.h
//  desassembleur
//
//  Created by Hubert Godfroy on 17/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_pool_h
#define desassembleur_pool_h

#include "LinkedList.h"
#include "registre.h"

typedef struct{
    Registre* reg;
    uint64_t val;
}Pool;

typedef struct _poolList{
    LinkedList pool;
}PoolList;

#endif
