//
//  testDyndesass.h
//  desassembleur
//
//  Created by Hubert Godfroy on 04/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_testDyndesass_h
#define desassembleur_testDyndesass_h

#include "../Fichier.h"
#include "../desassembleur.h"
#include "../loader.h"
#include "../commandes.h"

#define BINAIRE_BUILDTIME "../../tests/recc"
#define BINAIRE_RUNTIME "../../../../tests/recc"
#define MODELE_RUNTIME "../../../../tests/recc.dot"

void cfg_recc(void);
void cfg_entropie(void);
void cfg_disas(void);
void cfg_handbrake(void);

#endif
