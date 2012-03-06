#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <sys/mman.h>
//#include <sys/stat.h>

#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"

char* LOCAL = ".";

int main(int argc, char* argv []) {
    char chemin_vide[FILENAME_MAX];
    strcpy(chemin_vide, LOCAL);
    strcat(chemin_vide, "/vide.txt");
    Fichier* res_vide = newFichier(chemin_vide);
    cleanFile(res_vide);
    Fichier* binaire = newFichier("../../../../tests/recc");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    
    enregistrerVide(desas, res_vide);
    closeFichier(res_vide);
    closeFichier(binaire);
    terminateDesassembleur(desas);
    printf("done.\n");
    return 0;
}