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

#define EST_RECOUVERT 1
#define EST_LU 1

/*====== Valeurs possibles de type de liaison =====*/

#define TERMINAISON 0
#define JUMP_COND   1 
#define JUMP_INCOND 2
#define CALL        3
#define RET         4
#define FIN         5

/*========== Valeurs possibles d'interet =========*/

#define SANS_INTERET                 0
#define GO_AND_LEAVE                 1
#define DEBUT_FONCTION               2

#define OPCODE_INCONNU              -1
#define DEPASSEMENT_BLOC            -2

#define SAUT_INCOND_OUT_OF_BLOCK    -4
#define SAUT_INCOND_INDEFINI        -6

#define FIN_BLOC_SANS_POINT_ARRET   -7

#define CALL_TERMINAL               -3
#define CALL_FIN_BLOC               -5
#define CALL_INDEFINI               -12
#define CALL_OUT_OF_BLOCK           -13

#define SAUT_COND_FIN_BLOC          -8
#define SAUT_COND_INDEFINI          -9
#define SAUT_COND_OUT_OF_BLOCK      -10
#define SAUT_COND_TERMINAL          -11


typedef struct _Graphe{
    unsigned long VirtualAddrLue;       //adresse de l'instruction lue
//    unsigned long VirtualAddrPointee; //adresse de l'instruction pointée
    char interet;                        // voir les macros
    char typeLiaison;                    // voir les macros
    char assemble;                       //1 si neud déja vue (simplifieGraphe)
    char lu;                             // 1 si deja lu (dans buildGraphe)
    char affiche;                        // (afficheGraphe ou enregistreGraphe)
//    char debutFonction;                  // premiere inst d'une fonction
    char tailleInstruction;
    char recouvert;                      // 1 si PAS premier byte d'une instr
    
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;

#ifdef __cplusplus
}
#endif
    
#endif
