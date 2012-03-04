#ifndef desassembleur_dyndesass_h
#define desassembleur_dyndesass_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPPUSH 85
#define OPCALL 232
#define OP

#define AUTO_STEP 0
#define ALL_STEP 1

#include "LinkedList.h"
#include "Graphe.h"
#include "BeaEngine.h"
#include "Fichier.h"

extern char* LOCAL;

//#include <mach-o/loader.h>

typedef struct _desassembleur{
    DISASM* prog;
    unsigned long debutVirtuel;    // debut virtuel du bloc
}desasembleur;

void desassemblage_inconditionnel(DISASM* prog);

void fermeture(desasembleur* desas, Graphe pi[]);
Graphe* assembleGraphe(desasembleur* desas, Graphe pi[]);

Graphe* ControleFlow(DISASM* prog);
Graphe* ControleFlow2(desasembleur* desas);
Graphe* ControleFlow3(desasembleur* desas);

void afficheCF(Graphe* g);
void afficherPI(Graphe* pi, unsigned long taille);
void afficheCrible(int* crible, unsigned long taille, unsigned long pev);

void enregistreCF(Graphe* g, Fichier* tmp);

#endif
