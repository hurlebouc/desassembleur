//
//  testDesass.c
//  desassembleur
//
//  Created by Hubert Godfroy on 02/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include <stdio.h>

#include "../macro.h"

#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"

#include "testDyndesass.h"
#include "testVide.h"
#include "testOtpimizePool.h"
#include "CUnit/CUCurses.h"

char* ROOT;
char* TESTS;
int SYS = DESASSEMBLEUR_MAC;

#define TRUE 1
#define FALSE 0

int init_suite_success(void) { return 0; }
int clean_suite_success(void) { return 0; }


int main(int argc, char* argv [])
{
    
    ROOT = argv[0];
    TESTS = argv[1];
    printf("%lu\n", sizeof(Processeur));
    CU_pSuite pSuite = NULL;
    
    /*------------------------ INITIALISATION DU TESTEUR ---------------------*/
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /*---------------------------- CREATION DES TEST -------------------------*/
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("comparaison CFG", init_suite_success, clean_suite_success);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "CFG recc", cfg_recc))||
        (NULL == CU_add_test(pSuite, "CFG entropie", cfg_entropie))||
        (NULL == CU_add_test(pSuite, "CFG disas", cfg_disas))||
        (NULL == CU_add_test(pSuite, "CFG handbrake", cfg_handbrake)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("comparaison vide", init_suite_success, clean_suite_success);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "vide recc", vide_recc))||
        (NULL == CU_add_test(pSuite, "vide entropie", vide_entropie))||
        (NULL == CU_add_test(pSuite, "vide disas", vide_disas))||
        (NULL == CU_add_test(pSuite, "vide handbrake", vide_handbrake)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("comparaison log optimisation", init_suite_success, clean_suite_success);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "pool recc", optpool_recc))||
        (NULL == CU_add_test(pSuite, "pool entropie", optpool_entropie))||
        (NULL == CU_add_test(pSuite, "pool disas", optpool_disas))||
        (NULL == CU_add_test(pSuite, "pool handbrake", optpool_handbrake)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*--------------------------- LANCEMENT DES TESTS ------------------------*/
    
    /* Run all tests using the basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");
    
    /* Run all tests using the automated interface */
    //CU_automated_run_tests();
    //CU_list_tests_to_file();
    
    /* Run all tests using the console interface */
    //CU_console_run_tests();
    
    /* Run all tests using the curses interface */
    /* (only on systems having curses) */
    //CU_curses_run_tests();
    
    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}