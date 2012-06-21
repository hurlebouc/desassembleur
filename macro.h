//
//  macro.h
//  desassembleur
//
//  Created by Hubert Godfroy on 21/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_macro_h
#define desassembleur_macro_h

#define MAX_BUFFER 1024
#define CHEMIN_LOG_FERMETURE "buildGraphe.log"
#define CHEMIN_LOG_ASSEMBLAGE "simplifieGraphe.log"
#define CHEMIN_LOG_VIDE "vide.log"
#define CHEMIN_LOG_OPTIMISATION "optimisation.log"
#define DESASSEMBLEUR_LINUX 0
#define DESASSEMBLEUR_MAC 1
#define DEBUG_MODE
#define TAILLE_MEMOIRE 0x1000
#define ARCHI_PROC 64
#define EST_RECOUVERT 1

/*================= CONSTANTES ======================*/


enum armement_flag {
    FLAG_BAS,
    FLAG_HAUT,
    FLAG_NON_DEFINI = -1,
    FLAG_UNMODIFIED,
};

enum inclusion{
    INCLU_STRICTEMENT,
    EGAL,
    NON_INCLUS = -1,
};

enum delta{
    DELTA_BAISSE = 0,
    DELTA_LEVE,
    
};

enum casIndeterminisationMemoire {
    SEG_NON_INITIALISE =    0x1,
    SEG_INDETERMINEE =      0x2,
    SEG_NON_PRESENT =       0x4,
};

enum Classe {
    CLASSE_NON_DEFINIE = 0,
    CLASSE_DEFINI = 1,
};

enum Permission {
    LOCKED = 0,
    UNLOCKED,
};

enum TypeSystem{
    MACHO_64,
    ELF_32,
};

enum TypeVariable {
    ukn_type = 0,
    reg_type = 1,
    seg_type = 2,
};

/*== Valeurs possibles de _immediat (appels reccursifs)==*/

enum ValeurImmediat{
    NEANT,
    EST_LU,
    EST_ASSEMBLE,
    EST_AFFICHE,
    EST_LIBERE,
    PASSAGE_GET_NODE_WITH_VIRTUALADDR,
    PASSAGE_GET_NODE_WITH_VIRTUALADDR_U,
};

/*======== Valeurs possibles de type de liaison =======*/

enum ValeurLiaison{
    NOEUD_TERMINAISON,
    NOEUD_JUMP_COND,
    NOEUD_JUMP_INCOND,
    NOEUD_CALL,
    NOEUD_RET,
    NOEUD_EXIT,
};

/*============ Valeurs possibles d'interet ===========*/

enum ValeurEtat{
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

#endif
