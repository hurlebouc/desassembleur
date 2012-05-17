//
//  desassembleur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_desassembleur_h
#define desassembleur_desassembleur_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
    
#include "Fichier.h"
#include "LinkedList.h"
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine.h"
#include "processeur.h"
#include "liste_instr.h"
#include "pool.h"
    
    
    
    typedef struct _desassembleur{
        Processeur* proc;
        DISASM* disasm;
        unsigned long debutVirtuel;    // debut virtuel du bloc
    }Desasembleur;
    
    
    
    Desasembleur* newDesassembleur(Desasembleur*);
    void terminateDesassembleur(Desasembleur*);
    
    int desassemble(Desasembleur*);
    void litInstruction(Desasembleur*, PoolList*);
    
#ifdef __cplusplus
}
#endif

#endif
