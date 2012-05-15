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

Graphe* buildGraphe(Desasembleur* desas, Graphe pi[]);
Graphe* simplifieGraphe(Desasembleur* desas, Graphe* g);

//Graphe* ControleFlow(DISASM* prog);
//Graphe* ControleFlow2(desasembleur* desas);
Graphe* ControleFlow_entier(Desasembleur* desas);
Graphe* ControleFlow_simplifie(Desasembleur* desas);

void afficheGraphe(Graphe* g);
void afficherPI(Graphe* pi, unsigned long taille);
void afficheCrible(int* crible, unsigned long taille, unsigned long pev);

void enregistreGraphe(Graphe* g, Fichier* tmp);

#ifdef __cplusplus
}
#endif
    
#endif
