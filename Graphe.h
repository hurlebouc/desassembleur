//
//  Graphe.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef LL
#define LL
#include "LinkedList.h"
#endif

/*====== Valeurs possibles de type de liaison =====*/

#define TERMINAISON 0
#define JUMP_COND 1 
#define JUMP_INCOND 2
#define CALL 3
#define RET 4
#define FIN 5

/*========== Valeurs possibles d'interet =========*/

#define SANS_INTERET 0
#define GO_AND_LEAVE 1

#define OPCODE_INCONNU -1
#define DEPASSEMENT_BLOC -2

#define SAUT_INCOND_OUT_OF_BLOCK -4
#define SAUT_INCOND_INDEFINI -6

#define FIN_BLOC_SANS_POINT_ARRET -7

#define CALL_TERMINAL -3
#define CALL_FIN_BLOC -5
#define CALL_INDEFINI -12
#define CALL_OUT_OF_BLOCK -13

#define SAUT_COND_FIN_BLOC -8
#define SAUT_COND_INDEFINI -9
#define SAUT_COND_OUT_OF_BLOCK -10
#define SAUT_COND_TERMINAL -11


typedef struct _Graphe{
    unsigned long VirtualAddrLue;       // donne l'adresse de l'instruction lorsque le desassembleur passe dessus
    unsigned long VirtualAddrPointee;   // donne l'adresse de l'instruction lorsque le desassembleur pointe dessus
    int interet;                        // voir les macros
    int typeLiaison;                    // voir les macros
    int assemble;                       // 1 si le graphe fait deja partie du graphe final (assemblage)
    int lu;                             // 1 si cette instruction a deja ete lu (defini dans reperageAppels (déprédié) ou fermeture)
    int affiche;                        // ne sert que dans la fonction d affichage du graphe
    int debutFonction;                  // indique si l'indruction est le début d'une fonction (défini dans fermeture ou reperageAppels)
    LinkedList* listeFils;
    LinkedList* listePeres;
}Graphe;