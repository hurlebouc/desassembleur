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
        REGISTRE_NON_DEFINI = 0,
        REGISTRE_DEFINI,
    };
    /**
     * Registre virtuel
     */
    typedef struct _registre{ // 32 bytes
        int                 taille;/*!<Taille du registre*/
        uint64_t            valeur; // (seulement en cas de feuille)
        int                 classe;/*!<Classe d'appartenance de la méthode de propagation des constantes*/
        struct _registre*   filsl; /*!<registre correspondant aux 16 dernier bytes*/
        struct _registre*   filsh;/*!<registre correspondant aux 16 premiers bytes*/
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
