//
//  loader.h
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_loader_h
#define desassembleur_loader_h

#ifdef __cplusplus
extern "C" {
#endif
    
    
#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#ifdef __APPLE__
#include "LoaderMach.h"
#endif
//#ifdef __linux__
#include "loaderElf.h"
//#endif
    
    extern int SYS;
    
    void load(desasembleur* desas, Fichier* fichier);
    
#ifdef __cplusplus
}
#endif

#endif