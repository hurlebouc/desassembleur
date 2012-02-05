#ifndef LL
#define LL
#include "LinkedList.h"
#endif

#include <stdio.h>
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

/**
 * listePI est la liste des points d interet, c est a dire les points 
 * etant soit depart soit arrive d un saut.
 * les elements de la liste sont du type Graphe
 */
void lectureLineaire(DISASM* prog, LinkedList* listePI);


Graphe* ControleFlow(LinkedList* listePI);