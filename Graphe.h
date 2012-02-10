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
#define JUMP_INCOND 2 //contient aussi les call
#define TERMINAISON 0
#define RET 4


typedef struct _Graphe{
    unsigned long VirtualAddrLue;       // donne l'adresse de l'instruction lorsque le desassembleur passe dessus
    unsigned long VirtualAddrPointee;   // donne l'adresse de l'instruction lorsque le desassembleur pointe dessus
    int interet; // 1 si est le depart ou l arrivee d une fleche
    int typeLiaison; // JUMP_COND, JUMP_INCOND ou TERMINAISON
    int assemble; // 1 si le graphe fait deja parie du graphe final (assemblage)
    int lu; // 1 si cette instruction a deja ete lu (reperageAppels)
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;