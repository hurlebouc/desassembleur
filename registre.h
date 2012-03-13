//
//  registre.h
//  desassembleur
//
//  Created by Hubert Godfroy on 13/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur_registre_h
#define desassembleur_registre_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct _registre{
        int                 taille;
        uint64_t            valeur; // contient la valeur du Registre
        struct _registre*   filsl; 
        struct _registre*   filsh;
    }Registre;
    
    
    Registre* newRegistreFeuille(int taille, uint64_t valeur);
    Registre* newRegistreFilsUnique(Registre* filsl);
    Registre* newRegistreDeuxFils(Registre* filsh, Registre* filsl);
    
    void terminateRegistre(Registre* reg);
    
    uint64_t getValeur(Registre* reg);
    uint64_t setValeur(Registre* reg, uint64_t n); // retourne ce qu'il reste à attribuer
    void copieVal(Registre* dest, Registre* src);
    
//    /*- REMARQUES SUR LES FONCTIONS SUIVANTES -*/
//    
//    /* Lorsque les parametres des instructions ne sont pas des registres 
//     * on rentrera la valeurs dans des registres factices de taille maximum
//     * On évite ainsi d'utiliser des fonctions à tour larigot...
//     */
//    
//    void _and(registre* destination, registre* masque); //{destion} &= {masque}
//    void _add(registre* a, registre* b); 
//    void _move(registre* destination, registre* source);
//    void _lea(registre* a, registre* b);
//    void _shl(registre* val); // shift left
//    void _shr(registre* val);
//    void _sub(registre* a, registre* b);
//    void _xor(registre* a, registre* b); //xor
//    
//    /*---------------- sauts -------------------*/
//    
//    void _call(int len, registre* adresse);
//    void _jmp(registre* adresse);
//    void _jne(int len, registre* adresse);
//    void _ja(int len, registre* adresse);
//    void _jb(int len, registre* adresse);
//    void _jbe(int len, registre* adresse);
//    void _je(int len, registre* adresse);
//    void _jg(int len, registre* adresse);
//    void _jle(int len, registre* adresse);
//    void _ret();
//    
//    /*----------------- flags ------------------*/
//    
//    void _cmp(int len, registre* a, registre* b);
//    
//    /*------------------ pile ------------------*/
//    
//    void _push(int len, registre* a);
//    void _pop(int len, registre* reg);
    
#ifdef __cplusplus
}
#endif


#endif
