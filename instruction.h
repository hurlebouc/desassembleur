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
    int zf_aux;
    int pf_aux;
    int sf_aux;
    void* f;        // f renvoie un pointeur sur le registre qu'il a modifié (autre que le registre de flags)
}instruction;

void do_instr(instruction*, Registre*, Registre*,int, Processeur*);

#endif
