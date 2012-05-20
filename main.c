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

int SYS = DESASSEMBLEUR_MAC;

int main(int argc, char* argv []) {
    
    test* t = init_test();
        
    printf("%d\n", do_test(t, 23));
    printf("%d\n", t->f(23));
        
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
    printf("construction\n");
    Graphe* g = ControleFlow_entier(desas);
    g->etat = EST_ASSEMBLE;
    printf("optimisation\n");
    optimizePool2(g, desas->proc);
    printf("recherche");
    Graphe* n = getNodeWithVirtualAddr(g, 0x100000e82);
    printf(" : noeud 0x%lx trouvé\n", n->VirtualAddr);
    printf("recherche");
    Graphe* nf = getNodeWithVirtualAddr(g, 0x100000e8d);
    printf(" : noeud 0x%lx trouvé\n", nf->VirtualAddr);
    printf("enregistrement\n");
    enregistreGraphe(g, res);
    printf("destruction\n");
    terminateGraphe(g);
    
    closeFichier(res);
    closeFichier(binaire);
    terminateDesassembleur(desas);
    
//    test* t = init_t();
//    printf("%d\n", do_test(t, 4));
    
    printf("done.\n");
    return 0;
}