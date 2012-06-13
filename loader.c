//
//  loader.c
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "loader.h"

void load(Desasembleur* desas, Fichier* fichier){
    
    if (SYS == DESASSEMBLEUR_MAC) {
        loaderMach(desas, fichier);
    } else {
        loaderElf(desas, fichier);
    }
}