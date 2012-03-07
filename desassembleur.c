//
//  desassembleur.c
//  desassembleur
//
//  Created by Hubert Godfroy on 05/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//


#include "desassembleur.h"

desasembleur* newDesassembleur(desasembleur* desas){
    
    if (desas == NULL) {
        desas = malloc(sizeof(desasembleur));
        desas->disasm = malloc(sizeof(DISASM));;
        desas->proc = malloc(sizeof(Processeur));
        desas->proc->pileAppel = newLinkedList();
    }
    /* ============================= met tous les champs à zéro (important !)*/
    (void) memset(desas->disasm, 0, sizeof (DISASM));
    
    return desas;
}

void terminateDesassembleur(desasembleur* desas){
    free(desas->disasm);
    terminateLinkedList(desas->proc->pileAppel); // on utilisera pas de lien vers l'extérieur
    free(desas->proc);
    free(desas);
}

int litInstruction(desasembleur* desas){
    int len = Disasm(desas->disasm);
    
    // Partie chiante ...
    
    return len;
}