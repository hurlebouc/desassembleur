//
//  testMemoire.h
//  desassembleur
//
//  Created by Hubert Godfroy on 01/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_testMemoire_h
#define desassembleur_testMemoire_h

#include "CUnit/Basic.h"
#include "../commandes.h"

#define BINAIRE_BUILDTIME "../../tests/recc"
#define BINAIRE_RUNTIME "../../../../tests/recc"
#define MODELE_RUNTIME "../../../../tests/recc.dot"


void mem_recc(void);
void mem_entropie(void);
void mem_disas(void);
void mem_handbrake(void);

#endif
