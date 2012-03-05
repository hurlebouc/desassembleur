//
//  commades.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_commades_h
#define desassembleur_commades_h


#include "dyndesass.h"
#include "Vide.h"

#include "CUnit/Basic.h"


void afficherCFG(desasembleur* desas);
void enregistrerCFG(desasembleur* desas, Fichier* tmp);
void afficherVide(desasembleur* desas);
void enregistrerVide(desasembleur* desas, Fichier* fichier);

#endif
