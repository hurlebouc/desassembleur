//
//  instruction.h
//  desassembleur
//
//  Created by Hubert Godfroy on 23/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//  
//  Cette classe a pour but de faire de la POO en C..
//  Pour cela, je vais tenter d'utiliser un paradigme de 
//  programmation fonctionnelle.
//

#ifndef desassembleur_instruction_h
#define desassembleur_instruction_h

#include "registre.h"
#include "processeur.h"

typedef struct _instruction{
    void* of_aux;
    void* cf_aux;
    void* af_aux;
    void* f;        // stock le resultat dans _res et ne fait rien d'autre
}instruction;

void do_instr(instruction*, Registre*, Registre*,int, Processeur*);

#endif
