//
//  desassembleur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/*!
 * @file desassembleur.h
 */

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
    
    
    /*!
     Structure qui va être utilisé à chaque étape du désassemblage. */
    typedef struct _desassembleur{
        DISASM* disasm;/*!< Structure nécéssaire à Beaengine pour désassembler */
        unsigned long debutVirtuel; /*!<Début virtuel du bloc */
        Processeur* proc; /*!<Processeur virtuel dans l'état avant instruction */
    }Desasembleur;
    
    /*!
     * Créer une structure Disasm utilisé par BeaEngine
     * @return La structure disasm
     */
    PDISASM newDisasm(void);
    
    /*!
     * Crée un nouveau déassembleur
     * @param[in] Désassembleur vide à doter d'un disasm entre autres
     * @return Le désassembleur
     */
    Desasembleur* newDesassembleur(Desasembleur*);
    
    /*!
     * Efface de la mémoire un Desassembleur
     * @param[in] Désassembleur à éffacer
     */
    void terminateDesassembleur(Desasembleur*);
    
    int desassemble(Desasembleur*);
    
#ifdef __cplusplus
}
#endif

#endif
