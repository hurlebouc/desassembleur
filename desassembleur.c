//
//  desassembleur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//


#include "desassembleur.h"
#include "_macro_Build.h"

PDISASM newDisasm(void){
    PDISASM disas = malloc(sizeof(DISASM));
    (void) memset(disas, 0, sizeof (DISASM));
    return disas;
}


Desasembleur* newDesassembleur(Desasembleur* desas){
    
    if (desas == NULL) {
        desas = malloc(sizeof(Desasembleur));
        desas->disasm = malloc(sizeof(DISASM));
        desas->proc = newProcesseur(TAILLE_MEMOIRE);
    }
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(desas->disasm, 0, sizeof (DISASM));
    
    return desas;
}

void terminateDesassembleur(Desasembleur* desas){
    free(desas->disasm);
    terminateProcesseur(desas->proc);
    free(desas);
}



int desassemble(Desasembleur* desas){
    int len = Disasm(desas->disasm);
    
    return len;
}