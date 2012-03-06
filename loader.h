//
//  loader.h
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_loader_h
#define desassembleur_loader_h

#include "Fichier.h"
#include "desassembleur.h"
#include "LoaderMach.h"
#include "loaderElf.h"

void load(desasembleur* desas, Fichier* fichier);

#endif