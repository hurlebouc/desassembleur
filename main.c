#include <stdio.h>
#include <expat.h>

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
    Fichier* binaire = newFichier(argv[1]);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, ELF_32);
    Graphe* g = ControleFlow_entier(desas);
    optimizePool2(g, desas->proc);
    enregistrePropagation(newFichier("pools.txt"), g);
    
    return 0;
}