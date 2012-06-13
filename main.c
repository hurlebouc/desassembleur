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

int SYS = DESASSEMBLEUR_LINUX;

int main(int argc, char* argv []) {
        
    ROOT = argv[0];
    
    Fichier* binaire = newFichier("/Users/hubert/Desktop/a.out");
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe* g = ControleFlow_simplifie(desas);
    enregistreGraphe(g, newFichier("prog.dot"));
    return 0;
}