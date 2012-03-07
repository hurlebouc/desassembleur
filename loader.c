//
//  loader.c
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "loader.h"

void load(desasembleur* desas, Fichier* fichier){
    
#ifdef __APPLE__
    loaderMach(desas, fichier);
#endif
#ifdef __linux__
    loaderElf(desas, fichier);
#endif
}