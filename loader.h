//
//  loader.h
//  desassembleur
//
//  Created by Hubert Godfroy on 06/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/**
 * @file loader.h
 */

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
    /**
     * Récupère le point d'entrée et met la structure sous la forme approprié pour
     * pouvoir commencer le désassemblage
     * @param[out] desas  Qui va être mis en forme adéquate
     * @param[in] fichier Executable qui a été copié dans un buffer
     */
    void load(Desasembleur* desas, Fichier* fichier);
    
#ifdef __cplusplus
}
#endif

#endif