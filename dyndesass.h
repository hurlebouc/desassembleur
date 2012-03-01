#ifndef desassembleur_dyndesass_h
#define desassembleur_dyndesass_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPPUSH 85
#define OPCALL 232
#define OP

#include "LinkedList.h"
#include "Graphe.h"
#include "BeaEngine.h"

//#include <mach-o/loader.h>

typedef struct _desassembleur{
    DISASM* prog;
    unsigned long debut;    // debut virtuel du bloc
}desasembleur;

void desassemblage_inconditionnel(DISASM* prog);

void fermeture(desasembleur* desas, Graphe pi[]);

Graphe* ControleFlow(DISASM* prog);
Graphe* ControleFlow2(desasembleur* desas);
Graphe* ControleFlow3(desasembleur* desas);

void afficheCF(Graphe* g);
void afficherPI(Graphe* pi, unsigned long taille);
void afficheCrible(int* crible, unsigned long taille, unsigned long pev);

#endif
