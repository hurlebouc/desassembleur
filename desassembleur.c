//
//  desassembleur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//


#include "desassembleur.h"

desasembleur* newDesassembleur(desasembleur* desas, Fichier* fichier){
    
    char* chemin = fichier->chemin;
    if (desas == NULL) {
        DISASM* prog = malloc(sizeof(DISASM));
        desas = malloc(sizeof(desasembleur));
        desas->prog = prog;
    }
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(desas->prog, 0, sizeof (DISASM));
    
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    /*============================= chargement en mémoire====================*/
    void* debut = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    
    /*======  c'est ici qu'il faut changer en fonction de l'architecture  =====*/
    
    unsigned long debutVirtuel = loaderMach(debut, desas->prog);
    
    /*=========================================================================*/
    
    close(fd);
    desas->debutVirtuel = debutVirtuel;
    return desas;
}