#include <stdio.h>

#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"
#include "instruction.h"
#include "liste_instr.h"

char* ROOT;

int main(int argc, char* argv []) {
        
    ROOT = argv[0];
    char* chemin = argv[2];
    int type;
    if (strcmp(chemin, "mac") == 0) {
        type = MACHO_64;
    } else {
        type = ELF_32;
    }
    Fichier* binaire = newFichier(argv[1]);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, type);
    printf("cfg\n");
    Graphe* g = ControleFlow_entier(desas);
    enregistreGraphe(g, newFichier("graphe.dot"));
//    printf("optimise\n");
//    optimizePool2(g, desas->proc);
//    printf("enregistre propagation\n");
//    enregistrePropagation(newFichier("pools.txt"), g);
    printf("elagage\n");
    elagage(g, desas->proc);
    enregistreGraphe(g, newFichier("graphe_optimisé.dot"));
    
    return 0;
}
