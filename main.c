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

char* ROOT = "./";

//static int g(int n){
//    return n*n;
//}

int main(int argc, char* argv []) {
    char chemin_vide[FILENAME_MAX];
    strcpy(chemin_vide, ROOT);
    strcat(chemin_vide, "vide.txt");
    Fichier* res_vide = newFichier(chemin_vide);
    cleanFile(res_vide);
    Fichier* binaire = newFichier("../../../../tests/recc");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    
    enregistrerVide(desas, res_vide);
    closeFichier(res_vide);
    closeFichier(binaire);
    terminateDesassembleur(desas);
    
//    void* f = g;
//    
//    printf("%d : taille : %lu\n", H(f), *g);
    
    printf("done.\n");
    return 0;
}