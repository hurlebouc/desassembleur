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
    
    /**
     * Créer un registre virtuel qui n'a pas de sous registres
     * @param[in] taille Taille du registre à creer
     * @param[in] valeur A VOIR AVEC HUBERT
     * @return Renvoie le registre sans fils crée
     */
    Registre* newRegistreFeuille(int taille, uint64_t valeur);
    /**
     * Crée un registre avec une seule sous partie (dernier bytes)
     * @param[in] filsl registre qui est la sous partie
     * @return Le registre crée associé à sa sous partie
     */
    Registre* newRegistreFilsUnique(Registre* filsl);
    
    /**
     * Créer un registre avec deux sous-registres
     * @param[in] filsh Registre qui est la sous partie représentée par les derniers bytes
     * @param[in] filsl Registre qui est la sous partie représentée par les premiers bytes
     * @return Le registre associé de ses deux sous parties
     */
    Registre* newRegistreDeuxFils(Registre* filsh, Registre* filsl);
    
    /**
     * Efface un registre virtuel de la mémoire
     * @param[in] reg Registre à éffacer
     */
    void terminateRegistre(Registre* reg);
    /**
     * Calcul la valeur stocké dans un registre
     * @param[in] reg Le registre dont on veut calculer la valeur
     * @return La valeur stocké dans le registre
     */
    uint64_t getValeur(const Registre* reg);
    
    /**
     * Le paramètre n est trop petit : il faut utiliser un type plus grand
     */
    
    /**
     * Stock une valeur dans un registre si cela est possible
     * @param[in/out] reg Registre dans lequel on veut stocker la valeur
     * @param[in] n Valeur à stocker
     * @return Un nombre différent de 0 si le registre est trop petit pour contenir la valeur
     */
    uint64_t setValeur(Registre* reg, uint64_t n); 
    /**
     * Recopie la valeur d'un registre dans un autre
     * @param[in/out] dest Registre dans lequel on stock la nouvel valeur
     * @param[in] src Registre dont on extrait la valeur à copier
     */
    void copieVal(Registre* dest, Registre* src);
    
    /**
     * Ajout à la valeur d'un registre
     * @param[in/out] reg Registre auquel on veut ajouter la valeur
     * @param[in] len Valeur à ajouter
     */
    void incr(Registre* reg, int len);
    
    /* ----------------------------- PROPAGATION --------------------------- */
    
    int getClasse(Registre*);
    
#ifdef __cplusplus
}
#endif


#endif
