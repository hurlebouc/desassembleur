/* 
 * File:   loaderElf.h
 * Author: jean
 *
 * Created on 3 janvier 2012, 23:25
 */

#ifndef desassembleur_loaderElf_h
#define desassembleur_loaderElf_h

#ifdef __cplusplus
extern "C" {
#endif

#define __LIBELF_INTERNAL__ 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libelf.h>
    
#include "processeur.h"
#include "Fichier.h"
#include "desassembleur.h"

/**
 * Loader spécifique aux fichier elf 32 bits
 * Cela va chercher le point d'entrée du programme, la taille de l'objet à 
 * desassembler.
 * @param[out] desas
 * @param[in] fichier
 */
void loaderElf(Desasembleur* desas, Fichier* fichier);

#ifdef __cplusplus
}
#endif
    
#endif