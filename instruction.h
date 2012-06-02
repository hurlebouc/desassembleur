//
//  instruction.h
//  desassembleur
//
//  Created by Hubert Godfroy on 23/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//  
//  Cette classe a pour but de faire de la POO en C..
//

/**
 * @file instruction.h
 */

#ifndef desassembleur_instruction_h
#define desassembleur_instruction_h

#include "variable.h"
#include "processeur.h"

enum Permission {
    LOCKED = 0,
    UNLOCKED,
    };

/**
 * Cette structure donne toutes les informations permettant de réaliser une 
 * instruction assembleur. Chacune des méthodes doivent également tenir compte du 
 * fait que certain de leurs paamètres peuvent être indéterminés.
 */
typedef struct _instruction{
    
    /**
     * Cette méthode indique comment doit être réglé le drapeau d'overflow
     * @param a 
     * @param b
     * @param c
     * @return La méthode renvoie un élément de armement_flag
     */
    int(* of_aux)(const Variable a, const Variable b, const Variable c);
    
    /**
     * Cette méthode indique comment doit être réglé le drapeau CF
     */
    int(* cf_aux)(const Variable a, const Variable b, const Variable c);
    
    /**
     * Cette méthode indique comment doit être réglé le drapeau CF
     */
    int(* af_aux)(const Variable a, const Variable b, const Variable c);
    int zf_aux;         /*!<UNLOKED si l'instruction modifie ZF*/
    int pf_aux;         /*!<UNLOKED si l'instruction modifie PF*/
    int sf_aux;         /*!<UNLOKED si l'instruction modifie SF*/
    
    /**
     * f renvoie un pointeur sur le registre qu'il
     * ne modifie par le registre de flags
     * par contre il doit modifier le registre IP.
     */
    Variable (*f)(Variable a, Variable b, Variable c, Processeur*p, int len); 
    
}Instruction;

        /**
         * Execute l'instruction virtuelement si possible et ses conséquentes sur les
         * divers flags. Enregistre le changement d'état
         * dans un processeur virtuel
         * @param Instruction à executer
         * @param Registe concerné
         * @param Registre concerné
         * @param Registre concerné
         * @param Taille de l'instruction en octet
         * @param Processeur virtuel
         */
Variable do_instr(Instruction*, Variable, Variable, Variable, int, Processeur*);

/**
 * Crée une instruction
 * Il vaut mieux ne pas utiliser directement cette fonction dans le cadre de 
 * l'utilisation pour le désassemblage. En effet, il vaut mieux utiliser des
 * fonctions d'initialisation déjà préparée qui simplifie la création d'une 
 * instruction
 * @param of Fonction qui agit sur le flag d'overflow
 * @param cf Fonction qui agit sur le flag de retenue
 * @param af Fonction qui agit sur le flag d'ajustement
 * @param zf 1 si le flag de zéro peut être modifié, 0 sinon
 * @param pf 1 si le flag de parité peut être modifié, 0 sinon
 * @param sf 1 si le flag de signe peut être modifié, 0 sinon
 */
Instruction* newInstruction(
                            int of(const Variable, 
                                   const Variable,
                                   const Variable),
                            int cf(const Variable, 
                                   const Variable,
                                   const Variable),
                            int af(const Variable, 
                                   const Variable,
                                   const Variable),
                            int zf,
                            int pf,
                            int sf, 
                            Variable f(Variable, 
                                        Variable, 
                                        Variable,
                                        Processeur*, int)
                            );

/**
 * Efface de la mémoire une Instruction.
 * @param Instruction à effacer
 */
void terminateInstruction(Instruction*);

/*-------------------------------------------------------------------*/

typedef struct _test{
    int (*f)(int);
}test;

int do_test(test*, int);
test* newTest(int f(int));

#endif

