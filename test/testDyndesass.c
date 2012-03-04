//
//  testDyndesass.c
//  desassembleur
//
//  Created by Hubert Godfroy on 04/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "../Fichier.h"
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#include "../BeaEngine.h"
#include "../LoaderMach.h"
#include "../loaderElf.h"
#include "../dyndesass.h"
#include "../Vide.h"

#include "CUnit/Basic.h"

#define BINAIRE_BUILDTIME "../../tests/recc"
#define BINAIRE_RUNTIME "../../../../tests/recc"
#define MODELE_RUNTIME "../../../../tests/recc.dot"

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

unsigned long initDISASM(DISASM* prog, Fichier* binaire){
    char* chemin = binaire->chemin;
    
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

void afficherCFG(desasembleur* desas){
    Graphe*g = ControleFlow3(desas);
    printf("\n");
    afficheCF(g);
}

void enregistrerCFG(desasembleur* desas, Fichier* tmp){
    Graphe*g = ControleFlow3(desas);
    enregistreCF(g, tmp);
}

void afficherFermeture(desasembleur* desas){
    unsigned long taille = desas->prog->SecurityBlock;
    Graphe* pi = calloc(sizeof(Graphe),taille);
    fermeture(desas, pi);
    LinkedList* lVides = newLLFromclassificationVides(pi, taille);
    afficherVides(lVides, taille);
    terminatelVides(lVides);
}

void test1(void){
    
    /********************** INITIALISATION *******************/
    
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    Fichier* modele = newFichier(MODELE_RUNTIME);
    Fichier* binaire = newFichier(BINAIRE_RUNTIME);
    
    DISASM MyDisasm;
    desasembleur desas;
    desas.prog = &MyDisasm;
    unsigned long debutBloc = initDISASM(&MyDisasm, binaire);
    desas.debutVirtuel = debutBloc;
    
    enregistrerCFG(&desas, temp);
    
    long diff = fequals(temp, modele);
    CU_ASSERT_EQUAL(diff, -1);
}












