#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#ifndef BEAENGINE
#define BEAENGINE
#include "BeaEngine.h"
#endif

#ifndef MACH
#define MACH
#include "LoaderMach.h"
#endif

#ifndef DYN
#define DYN
#include "dyndesass.h"
#endif

//#include <mach-o/loader.h>

/**
 * Cette fonction desassemble statiquement un binaire.
 * 
 * Pour le moment, elle ne marche pas car le desassemeblage commence des le debut
 * du fichier sans tenir compte des headers.
 * 
 * De plus, une fois finie, cette fonction ne sera pas portable car elle dependra 
 * de la structure de binaire pour chaque OS (Mach-O par Mac, EFL pour Linux, ...)
 * 
 * ATTENTION : pour le moment, aucune borne n est implementee
 * (la terminaison n est pas verifiee)
 * 
 * utiliser #include <mach-o/loader.h>
 * 
 * @param chemin chemin du bianire
 */

void initialiserDISASM(DISASM* prog, char* chemin){
    
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(prog, 0, sizeof (DISASM));
    
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    
    /*============================= chargement en mémoire====================*/
    void* debut = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    
    /*======  c'est ici qu'il faut changer en fonction de l'architecture  =====*/
    
    loaderMach(debut, prog);
    
    /*=========================================================================*/
    
    close(fd);
}

void afficherCFG(DISASM* prog){
    Graphe*g = ControleFlow2(prog);
    printf("\n");
    afficheCF(g);
}

void afficherFermeture(DISASM* prog){
    unsigned long taille = prog->SecurityBlock;
    unsigned long pev = prog->VirtualAddr;
    int* crible = calloc(sizeof(int), taille);
    fermeture(prog, crible);
    afficheCrible(crible, taille, pev);
}

int main(int argc, char* argv []) {
    
    DISASM MyDisasm;
    char* chemin = "/Users/Hubert/Desktop/recc";
    initialiserDISASM(&MyDisasm, chemin);
    
    afficherCFG(&MyDisasm);
    
    //munmap(debut, size);
    
    return 0;
}