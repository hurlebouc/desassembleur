//
//  loader.c
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "loader.h"

void load(Desasembleur* desas, Fichier* fichier, int sys){
    switch (sys) {
        case MACHO_64:
            desas->proc->archi = 64;
            loaderMach(desas, fichier);
            break;
        case ELF_32:
            desas->proc->archi = 32;
            loaderElf(desas, fichier);
            break;
        default:
            break;
    }
}