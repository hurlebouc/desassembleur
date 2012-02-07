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

#define JUMP_COND 1 //contient aussi les call
#define JUMP_INCOND 2 // contient aussi les ret
#define TERMINAISON 0


typedef struct _Graphe{
    int interet;
    int typeLiaison;
    int visite;
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;