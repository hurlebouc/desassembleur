#include <stdio.h>

#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"
#include "instruction.h"
#include "liste_instr.h"

char* ROOT;

int SYS = DESASSEMBLEUR_MAC;

int main(int argc, char* argv []) {
    
    ROOT = argv[0];
    char* test = argv[1];
    
    char chemin_res[FILENAME_MAX];
    strcpy(chemin_res, ROOT);
    strcat(chemin_res, "graphe.dot");
    Fichier* res = newFichier(chemin_res);
    cleanFile(res);
    
    Fichier* binaire = newFichier(test);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    Graphe* g = ControleFlow_simplifie(desas);
    g->assemble = 1;
    enregistreGraphe(g, res);
    
    closeFichier(res);
    closeFichier(binaire);
    terminateDesassembleur(desas);
    
//    test* t = init_t();
//    printf("%d\n", do_test(t, 4));
    
    printf("done.\n");
    return 0;
}