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
#define _nR8L       45

#define _nR9        46
#define _nR9D       47
#define _nR9W       48
#define _nR9L       49

#define _nR10       50
#define _nR10D      51
#define _nR10W      52
#define _nR10L      53

#define _nR11       54
#define _nR11D      55
#define _nR11W      56
#define _nR11L      57

#define _nR12       58
#define _nR12D      59
#define _nR12W      60
#define _nR12L      61

#define _nR13       62
#define _nR13D      63
#define _nR13W      64
#define _nR13L      65

#define _nR14       66
#define _nR14D      67
#define _nR14W      68
#define _nR14L      69

#define _nR15       70
#define _nR15D      71
#define _nR15W      72
#define _nR15L      73

#define _nCS        74
#define _nDS        75
#define _nSS        76
#define _nES        77
#define _nFS        78
#define _nGS        79

#define NOMBRE_FLAGS 9

#define _nCF        0    // carry flag (retenue)
#define _nPF        1    // parity flag
#define _nAF        2    // auxiliary flag (retenue aux)
#define _nZF        3    // zero flag
#define _nSF        4    // sign flag
#define _nTF        5    // trap flag (debugage)            -> non implémenté
#define _nIF        6    // interrupt flag                  -> non implémenté
#define _nDF        7    // direction flag (chaine)         -> non implémenté
#define _nOF        8    // overflow flag

#define PILE_NON_DEFINIE NULL

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
