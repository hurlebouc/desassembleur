//
//  Vide.c
//  desassembleur
//
//  Created by Hubert Godfroy on 01/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include "Vide.h"

LinkedList* recherche(LinkedList* lVides, unsigned long taille){
    if (lVides->longueur == 0) {
        return NULL;
    }
    LinkedList* tete = lVides;
    while (tete != NULL && ((Vide*)((LinkedList*)tete->valeur)->valeur)->taille != taille) {
        tete = tete->suiv;
    }
    if (tete == NULL) {
        return NULL;
    }
    return tete;
}

void terminatelVides(LinkedList* lVides){
    LinkedList* tete = lVides;
    for (unsigned long i = 0; i<sizeLL(lVides); i++) {
        terminateLinkedListTotal(tete->valeur);
        tete = tete->suiv;
    }
    terminateLinkedList(lVides);
}

LinkedList* newLLFromclassificationVides(Graphe pi[], unsigned long taille){
    LinkedList* lVides = newLinkedList(); /* liste des listes de vides */
    unsigned long debutVirtuel = pi->VirtualAddrLue;
    unsigned long i = 0;
    while (i<taille) {
        if (!pi[i].lu) {
            unsigned long debutVide = i;
            Vide* v = malloc(sizeof(Vide));
            v->debut = debutVirtuel+i;
            i++;
            while (i<taille && (!pi[i++].lu) ) {}
            v->taille = i-debutVide - 1;
            printf("trou de 0x%lx à 0x%lx\n", v->debut, v->debut + v->taille-1);
            LinkedList* container = recherche(lVides, v->taille);
            if (container == NULL) {
                LinkedList* lClasseVides = newLinkedList();
                addLastLL(lClasseVides, v);
                addLastLL(lVides, lClasseVides);
            } else {
                LinkedList* lClasseVides = container->valeur;
                addLastLL(lClasseVides, v);
            }
        } else {
            i++;
        }
    }
    return lVides;
}

void afficherVides(LinkedList* lVides, unsigned long tailleProgramme){
    LinkedList* tete = lVides;
    float taille = tailleProgramme;
    float totvide = 0;
    for (unsigned long i = 0; i<lVides->longueur; i++) {
        float n = sizeLL((LinkedList*) tete->valeur);   // nombre de vides pour une taille donnée
        Vide* v = ((LinkedList*) tete->valeur)->valeur; // premier vide des vides d'une taille donnée
        unsigned long tailleVide = v->taille;           // taille des vides
        totvide += n*tailleVide;
        printf("%f pourcents du programme est vides par blocs de taille %lu\n", n/taille*100*tailleVide, tailleVide);
        tete = tete->suiv;
    }
    printf("%f du programme est vide\n", totvide/taille*100);
}
