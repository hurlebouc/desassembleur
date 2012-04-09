#include <stdio.h>

#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"
#include "instruction.h"
#include "liste_instr.h"

char* ROOT = "./";
int SYS = DESASSEMBLEUR_MAC;

int main(int argc, char* argv []) {
    char chemin_vide[FILENAME_MAX];
    strcpy(chemin_vide, ROOT);
    strcat(chemin_vide, "vide.txt");
    Fichier* res_vide = newFichier(chemin_vide);
    cleanFile(res_vide);
    Fichier* binaire = newFichier("/home/phil/Informatique2a/Projet2A/ElfDecryptage/switch");
    
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