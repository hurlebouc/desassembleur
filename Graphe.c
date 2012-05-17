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
    g->tailleInstruction = 0;
    g->recouvert = 0;
    g->listeFils = NULL;
    g->listePeres = NULL;
    return g;
}

void terminateGrapheSimple(Graphe* g){
    if (g->listeFils != NULL) {
        terminateLinkedList(g->listeFils);
    }
    if (g->listePeres != NULL) {
        terminateLinkedList(g->listePeres);
    }
//    printf("libération de %lx\n", g->VirtualAddrLue);
    free(g);
}

void terminateGraphe(Graphe* g){
    // Il faut eviter que se faire supprimer comme étant
    // un (arriere petit) fils de ses fils.
    if (g->lu == EST_LIBERE) {
        return;
    }
    g->lu = EST_LIBERE;
    
    // suppression de la liste des fils de chaque pere
    if (g->listePeres != NULL) {
//        printf("%lx : peres NULL\n", g->VirtualAddrLue);
        while (sizeLL(g->listePeres) != 0) {
            Graphe* pere = getFirstLL(g->listePeres);
            removeLink(pere, g);
        }
    }
    
    // effacement de chaque fils
    /*
     * Rq : la liste des fils de g diminue toute seule car les fils de g s'y 
     * suppriment au fur et à mesure qu'ils s'effacent.
     */
    if (g->listeFils != NULL) {
        while (sizeLL(g->listeFils) != 0) {
            Graphe* fils = getFirstLL(g->listeFils);
            terminateGraphe(fils);
        }
    }
    if (g->listeFils != NULL) {
        terminateLinkedList(g->listeFils);
    }
    if (g->listePeres != NULL) {
        terminateLinkedList(g->listePeres);
    }
//    printf("libération de %lx\n", g->VirtualAddrLue);
    free(g);
}

void removeLink(Graphe* pere, Graphe* fils){
    removeElementLL(pere->listeFils, fils);
    removeElementLL(fils->listePeres, pere);
}

void addLink(Graphe* pere, Graphe* fils){
    if (pere->listeFils == NULL) {
        pere->listeFils = newLinkedList();
    }
    if (fils->listePeres == NULL) {
        fils->listePeres = newLinkedList();
    }
    addFirstLL(pere->listeFils, fils);
    addFirstLL(fils->listePeres, pere);
}

static void opimizePool_aux(Graphe* g){
    
}

void optimizePool(Graphe* g){
    
}