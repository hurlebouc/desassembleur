#ifndef LL
#define LL
#include "LinkedList.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPPUSH 85
#define OPCALL 232
#define OP

#ifndef GRAPHE
#define GRAPHE
#include "Graphe.h"
#endif

#ifndef BEAENGINE
#define BEAENGINE
#include "BeaEngine.h"
#endif

//#include <mach-o/loader.h>

void desassemblage_inconditionnel(DISASM* prog);

void fermeture(DISASM* prog, int* crible);

Graphe* ControleFlow(DISASM* prog);
Graphe* ControleFlow2(DISASM* prog);

void afficheCF(Graphe* g);
void afficherPI(Graphe* pi, unsigned long taille);