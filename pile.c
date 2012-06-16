//
//  pile.c
//  desassembleur
//
//  Created by Hubert Godfroy on 16/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include "pile.h"


Stack* newStack(Memoire* mem){
    Stack* res = malloc(sizeof(Stack));
    res->pile = newLinkedList();
    res->mem = mem;
    return res;
}

void pushStack(Stack*stack, Variable*var, Registre* RSP){
    Segment* seg = getFirstLL(stack->pile);
}