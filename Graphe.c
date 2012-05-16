//
//  Graphe.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "Graphe.h"

Graphe* newGraphe(){
    Graphe* g = malloc(sizeof(Graphe));
    g->VirtualAddrLue = 0;
    g->aif = 0;
    g->interet = 0;
    g->typeLiaison = 0;
    g->assemble = 0;
    g->lu = 0;
    g->affiche = 0;
    g->tailleInstruction = 0;
    g->recouvert = 0;
    g->listeFils = NULL;
    g->listePeres = NULL;
    return g;
}

void terminateGraphe(Graphe* g){
    free(g);
}

static void opimizePool_aux(Graphe* g){
    
}

void optimizePool(Graphe* g){
    
}