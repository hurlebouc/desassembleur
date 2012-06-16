//
//  pile.h
//  desassembleur
//
//  Created by Hubert Godfroy on 16/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_pile_h
#define desassembleur_pile_h
#include <stdint.h>
#include "LinkedList.h"
#include "variable.h"

/* une pile contient des segments mémoire */
typedef struct{
    LinkedList* pile;
    Memoire* mem;
}Stack;

Stack* newStack(Memoire*);
uint64_t getStackVal(Stack*);
int getStackClass(Stack*);
void pushStack(Stack*, Variable*, Registre* _RSP);
void popStack(Stack*, Variable*, Registre* _RSP);

#endif
