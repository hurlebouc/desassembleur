//
//  loader.c
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "loader.h"

void load(Desasembleur* desas, Fichier* fichier, int sys){
    
    if (sys == MACHO_64) {
        loaderMach(desas, fichier);
    } else {
        loaderElf(desas, fichier);
    }
}