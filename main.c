#include <stdio.h>

#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"
#include "instruction.h"
#include "liste_instr.h"

char* ROOT;
char* TESTS;

int SYS = DESASSEMBLEUR_MAC;

int main(int argc, char* argv []) {
    
    ROOT = argv[0];
    TESTS = argv[1];
    
    char chemin_vide[FILENAME_MAX];
    strcpy(chemin_vide, ROOT);
    strcat(chemin_vide, "vide.txt");
    Fichier* res_vide = newFichier(chemin_vide);
    cleanFile(res_vide);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "recc");
    Fichier* binaire = newFichier(chemin_bin);
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    
    enregistrerVide(desas, res_vide);
    closeFichier(res_vide);
    closeFichier(binaire);
    terminateDesassembleur(desas);
    
    test* t = init_t();
    printf("%d\n", do_test(t, 4));
    
    printf("done.\n");
    return 0;
}