//
//  Vide.h
//  desassembleur
//
//  Created by Hubert Godfroy on 01/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_Vide_h
#define desassembleur_Vide_h

#include "LinkedList.h"
#include "Graphe.h"
#include <stdlib.h>

typedef struct _Vide{
    unsigned long taille;
    unsigned long debut;
}Vide;

LinkedList* newLLFromclassificationVides(Graphe pi[], unsigned long taille);
void afficherVides(LinkedList* lVides, unsigned long tailleProgramme);
void terminatelVides(LinkedList* lVides);

#endif