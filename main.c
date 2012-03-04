#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine.h"
#include "LoaderMach.h"
#include "loaderElf.h"
#include "dyndesass.h"
#include "Vide.h"

#define CHEMIN_RUNTIME "../../../../tests/recc"
char* LOCAL = ".";

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

static unsigned long initialiserDISASM(DISASM* prog, char* chemin){
    
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(prog, 0, sizeof (DISASM));
    
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    
    /*============================= chargement en mémoire====================*/
    void* debut = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    
    /*======  c'est ici qu'il faut changer en fonction de l'architecture  =====*/
    
    unsigned long debutVirtuel = loaderMach(debut, prog);
    
    /*=========================================================================*/
    
    close(fd);
    return debutVirtuel;
}

static void afficherCFG(desasembleur* desas){
    Graphe*g = ControleFlow3(desas);
    printf("\n");
    afficheCF(g);
}

static void enregistrerCFG(desasembleur* desas, Fichier* tmp){
    Graphe*g = ControleFlow3(desas);
    enregistreCF(g, tmp);
}

static void afficherVide(desasembleur* desas){
    unsigned long taille = desas->prog->SecurityBlock;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    afficherVides(lVides, taille);
    terminatelVides(lVides);
}

static void enregistrerVide(desasembleur* desas, Fichier* fichier){
    unsigned long taille = desas->prog->SecurityBlock;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    enregistrerVides(lVides, taille, fichier);
    terminatelVides(lVides);
}

int main(int argc, char* argv []) {
    char chemin_dot[FILENAME_MAX];
    strcpy(chemin_dot, LOCAL);
    strcat(chemin_dot, "/graphe.dot");
    Fichier* dot = newFichier(chemin_dot);
    cleanFile(dot);
    
    DISASM MyDisasm;
    desasembleur desas;
    desas.prog = &MyDisasm;
    unsigned long debutBloc = initialiserDISASM(&MyDisasm, CHEMIN_RUNTIME);
    desas.debutVirtuel = debutBloc;
    enregistrerCFG(&desas, dot);
    closeFichier(dot);
    
    char chemin_vide[FILENAME_MAX];
    strcpy(chemin_vide, LOCAL);
    strcat(chemin_vide, "/vide.txt");
    Fichier* res_vide = newFichier(chemin_vide);
    cleanFile(res_vide);
    
    debutBloc = initialiserDISASM(&MyDisasm, CHEMIN_RUNTIME);
    desas.debutVirtuel = debutBloc;
    enregistrerVide(&desas, res_vide);
    closeFichier(res_vide);
    printf("done.\n");
    return 0;
}