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


typedef struct _Graphe{
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;