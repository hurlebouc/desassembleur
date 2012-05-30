//
//  processeur.h
//  desassembleur
//
//  Created by Hubert Godfroy on 07/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

/**
 * @file processeur.h
 */

#ifndef desassembleur_processeur_h
#define desassembleur_processeur_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include "LinkedList.h"
#include "registre.h"
    
#define CF_POWER 1
#define PF_POWER 4 
#define AF_POWER 16
#define ZF_POWER 64
#define SF_POWER 128
#define TF_POWER 256
#define IF_POWER 512
#define DF_POWER 1024
#define OF_POWER 2048
    
#define NOMBRE_REGISTRES 80
    
#define _nRAX       0
#define _nEAX       1
#define _nAX        2
#define _nAH        3
#define _nAL        4
    
#define _nRBX       5
#define _nEBX       6
#define _nBX        7
#define _nBH        8
#define _nBL        9
    
#define _nRCX       10
#define _nECX       11
#define _nCX        12
#define _nCH        13
#define _nCL        14
    
#define _nRDX       15
#define _nEDX       16
#define _nDX        17
#define _nDH        18
#define _nDL        19
    
#define _nRDI       20
#define _nEDI       21
#define _nDI        22
#define _nDIL       23
    
#define _nRSI       24
#define _nESI       25
#define _nSI        26
#define _nSIL       27
    
#define _nRBP       28
#define _nEBP       29
#define _nBP        30
#define _nBPL       31
    
#define _nRSP       32
#define _nESP       33
#define _nSP        34
#define _nSPL       35
    
#define _nRIP       36
#define _nEIP       37
#define _nIP        38
    
#define _nRFLAGS    39 
#define _nEFLAGS    40 
#define _nFLAGS     41 
    
#define _nR8        42
#define _nR8D       43
#define _nR8W       44
#define _nR8B       45
    
#define _nR9        46
#define _nR9D       47
#define _nR9W       48
#define _nR9B       49
    
#define _nR10       50
#define _nR10D      51
#define _nR10W      52
#define _nR10B      53
    
#define _nR11       54
#define _nR11D      55
#define _nR11W      56
#define _nR11B      57
    
#define _nR12       58
#define _nR12D      59
#define _nR12W      60
#define _nR12B      61
    
#define _nR13       62
#define _nR13D      63
#define _nR13W      64
#define _nR13B      65
    
#define _nR14       66
#define _nR14D      67
#define _nR14W      68
#define _nR14B      69
    
#define _nR15       70
#define _nR15D      71
#define _nR15W      72
#define _nR15B      73
    
#define _nCS        74
#define _nDS        75
#define _nSS        76
#define _nES        77
#define _nFS        78
#define _nGS        79
    
#define NOMBRE_FLAGS 9
    
#define _nCF        0    // carry flag (retenue) ////////////////
#define _nPF        1    // parity flag
#define _nAF        2    // auxiliary flag (retenue aux) ////////
#define _nZF        3    // zero flag
#define _nSF        4    // sign flag
#define _nTF        5    // trap flag (debugage)
#define _nIF        6    // interrupt flag
#define _nDF        7    // direction flag (chaine)
#define _nOF        8    // overflow flag ///////////////////////
    
#define PILE_NON_DEFINIE NULL
    
    enum armement_flag {
        FLAG_BAS,
        FLAG_HAUT,
        FLAG_NON_DEFINI = -1,
    };
    
    enum inclusion{
        INCLU_STRICTEMENT,
        EGAL,
        NON_INCLUS = -1,
    };
    
    enum delta{
        DELTA_LEVE,
        DELTA_BAISSE,
    };
    /**
     * Modélisation du processeur
     */
    typedef struct _Processeur{
        LinkedList* stack;
        Registre*   tabRegistre[NOMBRE_REGISTRES];
        uint8_t     tabFlags[NOMBRE_FLAGS];
        
        int8_t delta; /**
                       * Ce terme permet de donner à un pool la propriété de ne 
                       * pas appartenir à l'ensemble des pools possibles
                       * (permet à tous les pools du graphe d'être visités au 
                       * moins une fois)
                       */
    }Processeur;
    /**
     * Crée et renvoi un nouveau procésseur virtuel
     * @return Processeur renvoi un procésseur non initialiser
     */
    Processeur* newProcesseur(void);
    /**
     * Crée une copie d'un processeur virtuel
     * @param[in] Processeur processeur à copier
     * @return le processeur copié
     */
    Processeur* newProcesseurCopy(const Processeur*);
    /**
     * Efface un processeur virtuel de la mémoire
     * @param[in] proc
     */
    void terminateProcesseur(Processeur* proc);
    
    
    /* ----------------------------- PROPAGATION --------------------------- */
    
    int incluDans(const Processeur* p1, const Processeur* p2);
    
    /**
     * Le l'intercection des deux pools se trouve dans le premier pool p1.
     * L'autre n'est pas modifié.
     */
    void inter(Processeur*p1, const Processeur*p2);
    
#ifdef __cplusplus
}
#endif

#endif


















