//
//  Graphe.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef LL
#define LL
#include "LinkedList.h"
#endif

#define JUMP_COND 1
#define JUMP_INCOND 2
#define SEQ 0


typedef struct _Graphe{
    int interet;
    int typeLiaison;
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;