//
//  instruction.h
//  desassembleur
//
//  Created by Hubert Godfroy on 23/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//  
//  Cette classe a pour but de faire de la POO en C..
//

#ifndef desassembleur_instruction_h
#define desassembleur_instruction_h

#include "registre.h"
#include "processeur.h"

typedef struct _instruction{
    int(* of_aux)(const Registre*, const Registre*, const Registre*);
    int(* cf_aux)(const Registre*, const Registre*, const Registre*);
    int(* af_aux)(const Registre*, const Registre*, const Registre*);
    //void* of_aux;
//    void* cf_aux;
//    void* af_aux;
    int zf_aux;
    int pf_aux;
    int sf_aux;
    Registre* (*f)(Registre*, Registre*, Registre*, Processeur*, int);
//    void* f;      // f renvoie un pointeur sur le registre qu'il a modifié 
                    // ne modifie par le registre de flags
                    // par contre il doit modifier le registre IP.
}Instruction;

Registre* do_instr(Instruction*, Registre*, Registre*,Registre*,int, Processeur*);
Instruction* newInstruction(
                            int of(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int cf(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int af(const Registre*, 
                                   const Registre*,
                                   const Registre*),
                            int zf,
                            int pf,
                            int sf, 
                            Registre* f(Registre*, 
                                        Registre*, 
                                        Registre*,
                                        Processeur*, int)
                            );
void terminateInstruction(Instruction*);

/*-------------------------------------------------------------------*/

typedef struct _test{
    int (*f)(int);
}test;

int do_test(test*, int);
test* newTest(int f(int));

#endif

