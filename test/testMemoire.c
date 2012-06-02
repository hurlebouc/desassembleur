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
    printf("depassement : %llx\n", setSegVal(seg(mem, 0x19843, 8), 0x394451928374));
    afficheMemoire(mem);
    printf("val seg : %llx\n", getSegVal(seg(mem, 0x19846, 3)));
    setByteClass(mem->tabBytes[4], CLASSE_NON_DEFINIE);
    printf("depassement : %llx\n", setSegVal(seg(mem, 0x19847, 6), 0x2348ef3374));
    afficheMemoire(mem);
    printf("val seg : %llx\n", getSegVal(seg(mem, 0x19846, 3)));
    printf("depassement : %llx\n", setSegVal(seg(mem, 0x200000, 6), 0x2348ef3374));
    afficheMemoire(mem);
    printf("val seg : %llx\n", setSegVal(seg(mem, 0x1984b, 5), 0xcaca));
    afficheMemoire(mem);
    printf("val seg : %llx\n", getSegVal(seg(mem, 0x1984b, 5)));
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