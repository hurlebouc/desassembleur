//
//  Graphe.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//
/** file Graphe.h
 */

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
    /**
     * Elément d'un graphe de flow. C'est en fait un noeud qui donne l'adresse 
     * de ses père et de ses fils.
     */
    typedef struct _Graphe{             //45 bytes
        unsigned long VirtualAddr;      /*!<Adresse de l'instruction lu*/
        uintptr_t aif;                  /*!<Adresse dans le fichier (offset)*/
        int8_t etat;                    /*!<Etat d'analyse du noeud*/
        int8_t typeLiaison;             /*!<Type de liaison avec les autres noeuds*/
        int8_t _immediat;               /*!<Caractéristique particulière du noeud*/
        int8_t tailleInstruction;       /*!<Taille de l'instruction*/
        int8_t recouvert;               /*!<Indique si l'octet fait partie d'une instruction, vaut 1 si oui*/
        Processeur* pool;               /*!<Pointeur vers la l'ensemble de classes d'équivalence utilisé pour la propagation des constantes*/
        LinkedList* listeFils;          /*!<Pointeur vers les noeuds succésseurs*/
        LinkedList* listePeres;         /*!<Pointeur vers les noeuds précédent*/
    }Graphe;
    
    /**
     * Crée une structure de type graphe initialisé sans caractéristiques particulière
     * @return Le graphe crée
     */
    Graphe* newGraphe(void);
    
    /**
     * 
     * @param g
     */
    void terminateGraphe(Graphe* g);
    /**
     * Efface un noeud du graphe en rompant tout les liens avec ses prédécesseurs.
     * Cette fonction ne devrait pas être utilisée hors de cette classe car elle
     * ne maintient pas la propriete d'accessibilite de tous les noeuds du graphe.
     * Il vaut mieux lui référer removeLinkRec()
     * et ses succésseurs
     * @param[in] g Noeud à éffacer
     */
    void terminateNoeud(Graphe* g);
    
    /**
     * Efface du graphe une liaison de succéssion.
     * @param[in] pere Noeud précédent dont on enlève le noeud fils de ses succésseurs
     * @param[in] fils Noeud suivant dont on enlève le noeud père de ses prédécésseurs
     */
    void removeLink(Graphe* pere, Graphe* fils);
    
    /**
     * Efface la liaison entre deux noeuds. Si cette liaison rends une partie du graphe
     * inaccessible, cette fonction supprime la partie inaccessible
     * @param[in] pere Noeud dont on veut enlever la liaison avec le noeud fils succésseur
     * @param[in] fils Noeud dont on supprime la liaison avec le noeud père précédent
     */
    void removeLinkRec(Graphe* pere, Graphe* fils);
    
    /**
     * Ajoute une liaison entre deux noeud du graphe. 
     * @param[in] pere Noeud auquel on ajoute le noeud fils en tant que succésseur
     * @param[in] fils Noeud auquel on ajoute le noeud père en tant que prédécesseur
     */
    void addLink(Graphe* pere, Graphe* fils);
    
    /**
     * Trouve un noeud du graphe en compléxité linaire
     * @param Graphe à parcourir
     * @param Virtuel addresse du noeuf à trouver
     * @return Pointeur vers le noeud recherché si trouvé, NULL sinon.
     */
    Graphe* getNodeWithVirtualAddr(Graphe*, uintptr_t);

    /**
     * Trouve un noeud du graphe en compléxité linaire. Cependant cette fonction
     * ne peut être utilisé qu'une seule fois par graphe.
     * @param Graphe à parcourir
     * @param Virtuel addresse du noeuf à trouver
     * @return Pointeur vers le noeud recherché si trouvé, NULL sinon.
     */    
    Graphe* getNodeWithVirtualAddrUnique(Graphe* g, uintptr_t va);
    
    /**
     * Cette fonction propage les constantes dans un graph à partir du pool 
     * initial. Chaque noeud contient les constantes APRÈS application de 
     * l'instruction
     * @param Graphe que l'on souhaite étudier
     * @param initialPool
     */
    void optimizePool(Graphe*, const Processeur* initialPool);
    
    /**
     * Cette fonction propage les constantes dans un graph à partir du pool 
     * initial. Chaque noeud contient les constantes AVANT application de 
     * l'instruction
     * @param Graphe que l'on souhaite étudier
     * @param initialPool
     */
    void optimizePool2(Graphe*, const Processeur* initialPool);
    
    /**
     * Cette fonction supprime les branches qui ne sont pas accessibles après
     * propagation des constantes
     * @param g
     */
    void debranchage(Graphe* g);

#ifdef __cplusplus
}
#endif
    
#endif
