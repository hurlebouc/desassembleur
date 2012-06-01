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
#include "../macro.h"

void mem_recc(void){
    Memoire* mem = newMemoire(1000);
    printf("depassement : %llx\n", setSegVal(mem, 0x19843, 8, UINT64_MAX));
    afficheMemoire(mem);
    printf("val seg : %llx\n", getSegVal(mem, 0x19846, 3));
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