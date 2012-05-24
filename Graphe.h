//
//  Graphe.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_Graphe_h
#define desassembleur_Graphe_h

#ifdef __cplusplus
extern "C" {
#endif

#include "LinkedList.h"
#include "desassembleur.h"
#include "macro.h"

#define EST_RECOUVERT 1
    
/*== Valeurs possibles de lecture (appels reccursifs)==*/
    
    enum ValeurImmediat{
        NEANT,
        EST_LU,
        EST_ASSEMBLE,
        EST_AFFICHE,
        EST_LIBERE,
        PASSAGE_GET_NODE_WITH_VIRTUALADDR,
        PASSAGE_GET_NODE_WITH_VIRTUALADDR_U,
    };
    
//#define EST_LU                              1
//#define EST_ASSEMBLE                        2
//#define EST_AFFICHE                         3
//#define EST_LIBERE                          4
//#define PASSAGE_GET_NODE_WITH_VIRTUALADDR   5
//#define PASSAGE_GET_NODE_WITH_VIRTUALADDR_U 6

/*======== Valeurs possibles de type de liaison =======*/
    
    enum ValeurLiaison{
        NOEUD_TERMINAISON,
        NOEUD_JUMP_COND,
        NOEUD_JUMP_INCOND,
        NOEUD_CALL,
        NOEUD_RET,
        NOEUD_EXIT,
    };

//#define TERMINAISON 0
//#define JUMP_COND   1 
//#define JUMP_INCOND 2
//#define CALL        3
//#define RET         4
//#define FIN         5

/*============ Valeurs possibles d'interet ===========*/
    
    enum ValeurInteret{
        SANS_INTERET,
        NOEUD_BRANCH,
        
        OPCODE_INCONNU,
        DEPASSEMENT_BLOC,
        
        SAUT_INCOND_OUT_OF_BLOCK,
        SAUT_INCOND_INDEFINI,
        
        FIN_BLOC_SANS_POINT_ARRET,
        
        CALL_TERMINAL_OOB,
        CALL_TERMINAL_INDEFINI,
        CALL_FIN_BLOC,
        CALL_INDEFINI,
        CALL_OUT_OF_BLOCK,
        
        SAUT_COND_FIN_BLOC,
        SAUT_COND_INDEFINI,
        SAUT_COND_OUT_OF_BLOCK,
        SAUT_COND_TERMINAL,
    };

//#define SANS_INTERET                 0
//#define GO_AND_LEAVE                 1
//#define DEBUT_FONCTION               2
//
//#define OPCODE_INCONNU              -1
//#define DEPASSEMENT_BLOC            -2
//
//#define SAUT_INCOND_OUT_OF_BLOCK    -4
//#define SAUT_INCOND_INDEFINI        -6
//
//#define FIN_BLOC_SANS_POINT_ARRET   -7
//
//#define CALL_TERMINAL               -3
//#define CALL_FIN_BLOC               -5
//#define CALL_INDEFINI               -12
//#define CALL_OUT_OF_BLOCK           -13
//
//#define SAUT_COND_FIN_BLOC          -8
//#define SAUT_COND_INDEFINI          -9
//#define SAUT_COND_OUT_OF_BLOCK      -10
//#define SAUT_COND_TERMINAL          -11
    
    extern char* ROOT;
    
    typedef struct _Graphe{             //45 bytes
        unsigned long VirtualAddr;      //adresse de l'instruction lue
        uintptr_t aif;                  // address in file
        int8_t etat;                    // voir les macros
        int8_t typeLiaison;             // voir les macros
        int8_t _immediat;               // voir les macros
        int8_t tailleInstruction;
        int8_t recouvert;               // 1 si PAS premier byte d'une instr
        Processeur* pool;               // propation de constantes
        LinkedList* listeFils;
        LinkedList* listePeres;
    }Graphe;
    
    Graphe* newGraphe(void);
        
    void terminateGraphe(Graphe* g);
    void terminateNoeud(Graphe* g);
    
    void removeLink(Graphe* pere, Graphe* fils);
    void removeLinkRec(Graphe* pere, Graphe* fils);
    void addLink(Graphe* pere, Graphe* fils);
    
    
    /**
     * Cette fonction trouve un noeud du graphe. 
     * Son temps de travail est en linéaire (aussi)
     */
    Graphe* getNodeWithVirtualAddr(Graphe*, uintptr_t);
    
    /**
     * Cette fonction trouve un noeud du graphe mais ne peut être utilisée 
     * qu'une seule fois sur ce graphe. Par contre elle est en temps linéaire.
     */
    Graphe* getNodeWithVirtualAddrUnique(Graphe* g, uintptr_t va);
    
    void optimizePool(Graphe*, const Processeur* initialPool);
    void optimizePool2(Graphe*, const Processeur* initialPool);
    
    void debranchage(Graphe* g);

#ifdef __cplusplus
}
#endif
    
#endif
