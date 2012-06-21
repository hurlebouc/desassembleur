//
//  processeur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/*!
 * @file processeur.h
 */

#ifndef desassembleur_processeur_h
#define desassembleur_processeur_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include "LinkedList.h"
#include "macro.h"
#include "registre.h"
#include "memoire.h"
#include "pile.h"
#include "Fichier.h"
    
    /*!
     * Modélisation du processeur
     */
    typedef struct _Processeur{
        uint8_t archi;
        Stack* stack;
        Memoire* mem;
        Registre*   tabRegistre[NOMBRE_REGISTRES];
        int8_t      tabFlags[NOMBRE_FLAGS];
        
        int8_t delta; /*!
                       * Ce terme permet de donner à un pool la propriété de ne 
                       * pas appartenir à l'ensemble des pools possibles
                       * (permet à tous les pools du graphe d'être visités au 
                       * moins une fois)
                       * Par defaut il est baissé.
                       */
    }Processeur;
    /*!
     * Crée et renvoi un nouveau processeur
     * @return Processeur renvoi un processeur non initialisé
     */
    Processeur* newProcesseur(uint64_t MemorySize);
    /*!
     * Crée une copie d'un processeur
     * @param[in] Processeur processeur à copier
     * @return le processeur copié
     */
    Processeur* newProcesseurCopy(const Processeur*);
    
    void initProcesseurCopy(Processeur* dest, const Processeur* src);
    
    /*!
     * Efface un processeur de la mémoire
     * @param[in] proc
     */
    void terminateProcesseur(Processeur* proc);
    
    
    /* ----------------------------- PROPAGATION --------------------------- */
    
    int incluDans(const Processeur* p1, const Processeur* p2,  Fichier* fichierlog);
    
    /*!
     * Le l'intercection des deux pools se trouve dans le premier pool p1.
     * L'autre n'est pas modifié.
     */
    void inter(Processeur*p1, const Processeur*p2);
    
    void enregistrerPool(Fichier*, Processeur*);
    
#ifdef __cplusplus
}
#endif

#endif


















