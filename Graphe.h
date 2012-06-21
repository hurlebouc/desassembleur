//
//  Graphe.h
//  desassembleur
//
//  Created by Hubert Godfroy on 05/02/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//
/*! 
 * @file Graphe.h
 */

#ifndef desassembleur_Graphe_h
#define desassembleur_Graphe_h

#ifdef __cplusplus
extern "C" {
#endif

#include "LinkedList.h"
#include "desassembleur.h"
#include "variable.h"
#include "macro.h"

#define EST_RECOUVERT 1
    
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
    
    extern char* ROOT;
    /*!
     * Elément d'un graphe de flow. C'est en fait un noeud qui donne l'adresse 
     * de ses père et de ses fils.
     */
    typedef struct _Graphe{         //45 bytes
        uint64_t VirtualAddr;  /*!<Adresse de l'instruction lu*/
        uintptr_t aif;              /*!<Adresse dans le fichier (offset)*/
        int8_t etat;                /*!<valeur de la liaison (erreurs)*/
        int8_t typeLiaison;         /*!<Type de liaison avec les autres noeuds*/
        int8_t _immediat;           /*!<Champs privé servant lors du traitement 
                                     des fonction récusives*/
        int8_t tailleInstruction;   /*!<Taille de l'instruction*/
        int8_t recouvert;           /*!<Indique si l'octet fait partie d'une
                                     instruction*/
        Processeur* pool;           /*!<Pointeur vers la l'ensemble de classes 
                                     d'équivalence utilisé pour la propagation 
                                     des constantes*/
        LinkedList* listeFils;      /*!<Pointeur vers les noeuds succésseurs*/
        LinkedList* listePeres;     /*!<Pointeur vers les noeuds précédent*/
    }Graphe;
    
    /*!
     * Crée une structure de type graphe initialisé sans caractéristiques 
     * particulière
     * @return Le graphe crée
     */
    Graphe* newGraphe(void);
    
    /*!
     * Fonction de destruction d'un graphe
     * @param g
     */
    void terminateGraphe(Graphe* g);
    
    /*!
     * Efface la liaison entre deux noeuds. Si cette liaison rends une partie du 
     * graphe inaccessible, cette fonction supprime la partie inaccessible
     * @param pere Noeud dont on veut enlever la liaison avec le noeud fils 
     * successeur
     * @param fils Noeud dont on supprime la liaison avec le noeud père précédent
     */
    void removeLinkRec(Graphe* pere, Graphe* fils);
    
    /*!
     * Ajoute une liaison entre deux noeud du graphe. 
     * @param[in] pere Noeud auquel on ajoute le noeud fils en tant que 
     * successeur
     * @param[in] fils Noeud auquel on ajoute le noeud père en tant que 
     * predecesseur
     */
    void addLink(Graphe* pere, Graphe* fils);
    
    /*!
     * Trouve un noeud du graphe en compléxité linaire
     * @param Graphe à parcourir
     * @param Virtuel addresse du noeuf à trouver
     * @return Pointeur vers le noeud recherché si trouvé, NULL sinon.
     */
    Graphe* getNodeWithVirtualAddr(Graphe*, uintptr_t);

    /*!
     * Trouve un noeud du graphe en compléxité linaire. Cependant cette fonction
     * ne peut être utilisé qu'une seule fois par graphe.
     * @deprecated
     * @param Graphe à parcourir
     * @param Virtuel addresse du noeuf à trouver
     * @return Pointeur vers le noeud recherché si trouvé, NULL sinon.
     */    
    Graphe* getNodeWithVirtualAddrUnique(Graphe* g, uintptr_t va);
    
    /*!
     * Cette fonction propage les constantes dans un graph à partir du pool 
     * initial. Chaque noeud contient les constantes APRÈS application de 
     * l'instruction
     * @deprecated
     * @param Graphe que l'on souhaite étudier
     * @param initialPool
     */
    void optimizePool(Graphe*, const Processeur* initialPool);
    
    /*!
     * Cette fonction propage les constantes dans un graph à partir du pool 
     * initial. Chaque noeud contient les constantes AVANT application de 
     * l'instruction
     * @param Graphe que l'on souhaite étudier
     * @param initialPool
     */
    void optimizePool2(Graphe*, const Processeur* initialPool);
    
    /*!
     * Cette fonction supprime les branches qui ne sont pas accessibles après
     * propagation des constantes
     * @param g
     */
    int debranchage_fils(Graphe* g);
    void elagage(Graphe*, Processeur* poolInit);
    
    /*!
     * Cette fonction désassemble l'instruction présente dans le noeud n. Elle
     * nécéssite la présence en mémoire du programme désassemblé.
     * @param n noeud contenant l'instruction
     */
    DISASM* newDisasmFromGraph(Graphe* n);
    
    /*!
     * Cette fonction renvoie le registre du pool donné en paramètre déterminé
     * par l'argument de type ARGTYPE
     * @param arg définition du registre par la syntaxe de Beaengine
     * @param newPool pool donc doit être extrait le registre
     * @return registre déterminé par arg dans newPool
     */
    Registre * getRegistre(ARGTYPE arg, Processeur *newPool);
    
    /*!
     * Cette fonction renvoie la constante déterminé par arg à la position du 
     * programme déterminé par disasm
     * @param arg définition de la constante par la syntaxe de Beaengine
     * @param disasm 
     * @return registre valeur de la constante sous forme de registre
     */
    Registre * getConstant(ARGTYPE arg, DISASM *disasm);
    
    Segment getMemory(ARGTYPE arg, /*DISASM* disasm,*/ Processeur* newPool);
    
    void enregistrePropagation(Fichier*, Graphe*);

#ifdef __cplusplus
}
#endif
    
#endif
