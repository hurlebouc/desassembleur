//
//  Vide.h
//  desassembleur
//
//  Created by Hubert Godfroy on 01/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_Vide_h
#define desassembleur_Vide_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "LinkedList.h"
#include "Graphe.h"
#include "Fichier.h"
extern char* LOCAL;

typedef struct _Vide{
    unsigned long taille;
    unsigned long debut;
}Vide;

LinkedList* newLLFromclassificationVides(Graphe pi[], unsigned long taille);
void afficherVides(LinkedList* lVides, unsigned long tailleProgramme);
void enregistrerVides(LinkedList* lVides, unsigned long tailleProgramme, Fichier* fichier);
void terminatelVides(LinkedList* lVides);

#ifdef __cplusplus
}
#endif
    
#endif
