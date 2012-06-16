//
//  pile.c
//  desassembleur
//
//  Created by Hubert Godfroy on 16/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include "pile.h"


Stack* newStack(){
    Stack* res = malloc(sizeof(Stack));
    res->pile = newLinkedList();
    return res;
}

void terminateStack(Stack* pile){
    terminateLinkedListTotal(pile->pile);
    free(pile);
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
    free(seg);
}

Stack* newStackCopy(Stack* src, Memoire* mem){
    if (src == NULL) {
        return NULL;
    }
    Stack* dest = newStack();
    LinkedList* tete = src->pile;
    uint64_t taille = sizeLL(tete);
    for (uint64_t i = 0; i<taille; i++) {
        Segment* segSrc = tete->valeur;
        Segment* segDest = newSegment(mem, segSrc->virtualAddr, segSrc->taille);
        addFirstLL(dest->pile, segDest);
    }
    return dest;
}