//
//  commades.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_commades_h
#define desassembleur_commades_h

#ifdef __cplusplus
extern "C" {
#endif

#include "dyndesass.h"
#include "loader.h"
#include "Vide.h"

void afficherCFG(Fichier* binaire, int sys);
void enregistrerCFG(Fichier* binaire, Fichier* dest, int sys);
void afficherVide(Desasembleur* desas);
void enregistrerVide(Desasembleur* desas, Fichier* fichier);

#ifdef __cplusplus
}
#endif
    
#endif
