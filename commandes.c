//
//  commandes.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "commandes.h"

void afficherCFG(desasembleur* desas){
    Graphe*g = ControleFlow3(desas);
    printf("\n");
    afficheCF(g);
}

void enregistrerCFG(desasembleur* desas, Fichier* tmp){
    Graphe*g = ControleFlow3(desas);
    enregistreCF(g, tmp);
}

void afficherVide(desasembleur* desas){
    unsigned long taille = desas->disasm->SecurityBlock + desas->disasm->VirtualAddr - desas->debutVirtuel;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    afficherVides(lVides, taille);
    terminatelVides(lVides);
}

void enregistrerVide(desasembleur* desas, Fichier* fichier){
    unsigned long taille = desas->disasm->SecurityBlock + desas->disasm->VirtualAddr - desas->debutVirtuel;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    enregistrerVides(lVides, taille, fichier);
    terminatelVides(lVides);
}