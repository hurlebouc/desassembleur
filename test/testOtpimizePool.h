//
//  testOtpimizePool.h
//  desassembleur
//
//  Created by Hubert Godfroy on 19/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_testOtpimizePool_h
#define desassembleur_testOtpimizePool_h

#include "CUnit/Basic.h"
#include "../commandes.h"

#define BINAIRE_BUILDTIME "../../tests/recc"
#define BINAIRE_RUNTIME "../../../../tests/recc"
#define MODELE_RUNTIME "../../../../tests/recc.dot"

void optpool_factice(void);

void optpool_recc(void);

void optpool_entropie(void);

void optpool_disas(void);

void optpool_handbrake(void);

#endif
