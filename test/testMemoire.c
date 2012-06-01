//
//  testMemoire.c
//  desassembleur
//
//  Created by Hubert Godfroy on 01/06/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//
extern char* TESTS;
extern char* ROOT;
#include <stdio.h>
#include "testMemoire.h"

void mem_recc(void){
    Memoire* mem = newMemoire(1000);
    setSegVal(mem, 19843, 4, 984);
    afficheMemoire(mem);
    CU_ASSERT_EQUAL(1, 1);
}
void mem_entropie(void){
    CU_ASSERT_EQUAL(1, 1);
}
void mem_disas(void){
    CU_ASSERT_EQUAL(1, 1);
}
void mem_handbrake(void){
    CU_ASSERT_EQUAL(1, 1);
}