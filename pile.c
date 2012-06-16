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

void pushStack(Stack*stack, Variable var, Registre* RSP){
    Segment* seg = getFirstLL(stack->pile);
    int tailleSegment = seg->taille; // en octets
    if (getVarTaille(var) > tailleSegment*8) {
        printf("la pile accueil des entiers de taille %d bits l'entrée est de  %d bits\n", tailleSegment*8, getVarTaille(var));
        exit(EXIT_FAILURE);
    }
    
    if (getRegClassRec(RSP) != CLASSE_NON_DEFINIE) {
        setRegVal(RSP, getRegVal(RSP) - tailleSegment);
    }
    
    uint64_t val = getVarVal(var);
    Segment* top = newSegment(seg->mem, seg->virtualAddr - tailleSegment, tailleSegment);
    
    setSegVal(*top, val);
    setSegClassRec(*top, getVarClassRec(var));
    addFirstLL(stack->pile, top);
}

void popStack(Stack* stack, Variable var, Registre* RSP){
    Segment* seg = getFirstLL(stack->pile);
    int tailleSegment = seg->taille; // en octets
    if (getVarTaille(var) > tailleSegment*8) {
        printf("la pile accueil des entiers de taille %d bits l'entrée est de  %d bits\n", tailleSegment*8, getVarTaille(var));
        exit(EXIT_FAILURE);
    }
    
    if (getRegClassRec(RSP) != CLASSE_NON_DEFINIE) {
        setRegVal(RSP, getRegVal(RSP) + tailleSegment);
    }
    
    if (getSegClassRec(*seg)[0] == CLASSE_NON_DEFINIE) {
        setVarClassRec(var, CLASSE_NON_DEFINIE);
        return;
    }
    
    uint64_t val = getSegVal(*seg);
    setVarVal(var, val);
    setVarClassRec(var, getSegClassRec(*seg)[0]);
    removeFirstLL(stack->pile);
    
}