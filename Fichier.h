//
//  Fichier.h
//  Entropie
//
//  Created by Hubert on 30/09/11.
//  Copyright 2011 Mines de Nancy. All rights reserved.
//

/**
 * \file Fichier.h
 * \author Hubert Godfroy
 * 
 * Implémente les fonctions de gestion de fichier
 */

#ifndef desassembleur_Fichier_h
#define desassembleur_Fichier_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define MAX_BUFFER 1024


typedef struct fichier {
    char* chemin;
} Fichier;

/**
 * Crée une nouvelle instance de fichier.
 * @param chemin
 */
Fichier* newFichier(char* chemin);

/**
 * Détruit le fichier.
 * @param f
 */
void closeFichier(Fichier* f);

/**
 * Ouvre le fichier en lecture.
 * @param f
 * @return fichier de type FILE* ouvert en lecture.
 */
FILE* ouvrirLecture(Fichier* f);

/**
 * Ouvre le fichier en écriture.
 * @param f
 * @return fichier de type FILE* ouvert en écriture
 */
FILE* ouvrirEcriture(Fichier* f);

/**
 * Compare deux fichier.
 * @param f1
 * @param f2
 * @return Renvoie l'index du premier caractère qui diffère entre les deux fichiers. 
 * Renvoie -1 sinon.
 */
long fequals(Fichier* f1, Fichier* f2);

/**
 * Écrit un tableau de caractères dans un fichier.
 * @param graveur
 * @param buffer
 */
void writeFlux(FILE* graveur, char* buffer, unsigned long taille);

/**
 * Lit autant de caractères dans le fichier que de case dans le tableau et stocke le resultat dans le buffer
 * @param lecteur
 * @param buffer
 */
void readFlux(FILE* lecteur, char* buffer, unsigned long taille);
//char* getPath(Fichier* f);
/**
 * Donne la taille en octets d'un fichier.
 * @param fichier
 */
unsigned long fsize(Fichier* fichier);

/**
 * Supprime un fichier.
 * @param fichier
 * @return retourne la valeur de remove(fichier->chemin)
 */
int removeFichier(Fichier* fichier);

void pushlog(Fichier* f, char* s);

void cleanFile(Fichier* f);

#ifdef __cplusplus
}
#endif

#endif