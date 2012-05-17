//
//  commandes.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "commandes.h"

void afficherCFG(Fichier* binaire){
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe*g = ControleFlow_simplifie(desas);
    afficheGraphe(g);
    terminateDesassembleur(desas);
}

void enregistrerCFG(Fichier* binaire, Fichier* tmp){
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe*g = ControleFlow_simplifie(desas);
    enregistreGraphe(g, tmp);
    terminateDesassembleur(desas);
    terminateGraphe(g);
}

void afficherVide(Desasembleur* desas){
    unsigned long taille = desas->disasm->SecurityBlock + desas->disasm->VirtualAddr - desas->debutVirtuel;
    Graphe** pi = calloc(sizeof(Graphe*),taille);
    buildGraphe(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    afficherVides(lVides, taille);
    terminatelVides(lVides);
}

void enregistrerVide(Desasembleur* desas, Fichier* fichier){
    unsigned long taille = desas->disasm->SecurityBlock + desas->disasm->VirtualAddr - desas->debutVirtuel;
    Graphe** pi = calloc(sizeof(Graphe*),taille);
    buildGraphe(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    enregistrerVides(lVides, taille, fichier);
    terminatelVides(lVides);
}