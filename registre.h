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
    
    enum DefinitionValeur {
        VALEUR_NON_DEFINIE = 0,
        VALEUR_DEFINIE,
    };
    
    typedef struct _registre{ // 32 bytes
        int                 taille;
        uint64_t            valeur; // (seulement en cas de feuille)
        int                 classe; // propagation des constantes
        struct _registre*   filsl; 
        struct _registre*   filsh;
    }Registre;
    
    
    Registre* newRegistreFeuille(int taille, uint64_t valeur);
    Registre* newRegistreFilsUnique(Registre* filsl);
    Registre* newRegistreDeuxFils(Registre* filsh, Registre* filsl);
    
    void terminateRegistre(Registre* reg);
    
    uint64_t getValeur(const Registre* reg);
    
    /**
     * Le paramètre n est trop petit : il faut utiliser un type plus grand
     */
    
    uint64_t setValeur(Registre* reg, uint64_t n); // retourne ce qu'il reste à attribuer
    void copieVal(Registre* dest, Registre* src);
    void incr(Registre* reg, int len);
    
    /* ----------------------------- PROPAGATION --------------------------- */
    
    int getClasse(Registre*);
    
#ifdef __cplusplus
}
#endif


#endif
