//
//  pile.c
//  desassembleur
//
//  Created by Hubert Godfroy on 16/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include "pile.h"


Stack* newStack(Memoire* mem, uint8_t taille){
    Stack* res = malloc(sizeof(Stack));
    res->pile = newLinkedList();
    res->mem = mem;
    res->taille = taille;
    return res;
}

void terminateStack(Stack* pile){
    terminateLinkedListTotal(pile->pile);
    free(pile);
}

void pushStack(Stack*stack, Variable var, Registre* RSP){
    if (getVarTaille(var) > stack->taille) {
        printf("ERROR : la pile accueil des entiers de taille %d bits l'entrée est de  %d bits\n", stack->taille, getVarTaille(var));
        exit(EXIT_FAILURE);
    }
    
    Segment* top = NULL;
    
    if (sizeLL(stack->pile) == 0) {
        if (getRegClassRec(RSP) != CLASSE_NON_DEFINIE) {
            top = newSegment(stack->mem, getRegVal(RSP), stack->taille);
        } else {
            return;
        }
    } else {
        Segment* seg = getFirstLL(stack->pile);
        top = newSegment(stack->mem, seg->virtualAddr - stack->taille, stack->taille);
    }
    
    
    if (getRegClassRec(RSP) != CLASSE_NON_DEFINIE) {
        setRegVal(RSP, getRegVal(RSP) - stack->taille);
    }
    setSegVal(*top, getVarVal(var));
    setSegClassRec(*top, getVarClassRec(var));
    addFirstLL(stack->pile, top);
}

void popStack(Stack* stack, Variable var, Registre* RSP){
    if (sizeLL(stack->pile) == 0) {
        printf("ERROR : La pile est vide et on ne peut dépiler.");
        exit(EXIT_FAILURE);
    }
    if (getVarTaille(var) > stack->taille) {
        printf("ERROR : la pile accueil des entiers de taille %d bits l'entrée est de  %d bits\n", stack->taille, getVarTaille(var));
        exit(EXIT_FAILURE);
    }
    
    Segment* seg = getFirstLL(stack->pile);
    
    if (getRegClassRec(RSP) != CLASSE_NON_DEFINIE) {
        setRegVal(RSP, getRegVal(RSP) + stack->taille);
    }
    
    if (getSegClassRec(*seg)[0] == CLASSE_NON_DEFINIE) {
        setVarClassRec(var, CLASSE_NON_DEFINIE);
        removeFirstLL(stack->pile);
        free(seg);
        return;
    }
    
    setVarVal(var, getSegVal(*seg));
    setVarClassRec(var, getSegClassRec(*seg)[0]);
    removeFirstLL(stack->pile);
    free(seg);
}

Stack* newStackCopy(Stack* src, Memoire* mem){
    if (src == NULL) {
        return NULL;
    }
    Stack* dest = newStack(mem, src->taille);
    LinkedList* tete = src->pile;
    uint64_t taille = sizeLL(tete);
    for (uint64_t i = 0; i<taille; i++) {
        Segment* segSrc = tete->valeur;
        Segment* segDest = newSegment(mem, segSrc->virtualAddr, segSrc->taille);
        addFirstLL(dest->pile, segDest);
    }
    return dest;
}