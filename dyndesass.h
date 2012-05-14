#ifndef desassembleur_dyndesass_h
#define desassembleur_dyndesass_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPPUSH 85
#define OPCALL 232
#define OP

#define AUTO_STEP 0
#define ALL_STEP 1

#include "macro.h"
#include "LinkedList.h"
#include "Graphe.h"
#include "desassembleur.h"
#include "Fichier.h"

extern char* ROOT;

//#include <mach-o/loader.h>

void desassemblage_inconditionnel(DISASM* prog);

void fermeture(desasembleur* desas, Graphe pi[]);
Graphe* simplifieGraphe(desasembleur* desas, Graphe pi[]);

//Graphe* ControleFlow(DISASM* prog);
//Graphe* ControleFlow2(desasembleur* desas);
Graphe* ControleFlow3(desasembleur* desas);

void afficheCF(Graphe* g);
void afficherPI(Graphe* pi, unsigned long taille);
void afficheCrible(int* crible, unsigned long taille, unsigned long pev);

void enregistreCF(Graphe* g, Fichier* tmp);

#ifdef __cplusplus
}
#endif
    
#endif
