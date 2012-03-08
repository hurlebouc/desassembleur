//
//  testVide.h
//  desassembleur
//
//  Created by Hubert Godfroy on 04/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_testVide_h
#define desassembleur_testVide_h

#include "CUnit/Basic.h"
#include "../commandes.h"

#define BINAIRE_BUILDTIME "../../tests/recc"
#define BINAIRE_RUNTIME "../../../../tests/recc"
#define MODELE_RUNTIME "../../../../tests/recc.dot"

void vide_recc(void);
void vide_entropie(void);
void vide_disas(void);
void vide_handbrake(void);

#endif
