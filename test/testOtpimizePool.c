//
//  testOtpimizePool.c
//  desassembleur
//
//  Created by Hubert Godfroy on 19/05/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

extern char* TESTS;

#include <stdio.h>
#include "testOtpimizePool.h"

void optpool_factice(void){
    
}

void optpool_recc(void){
    Fichier* log = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(log);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "recc_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "recc");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
       
    
    long diff = fequals(log, modele);
    if (diff == -1) {
        removeFichier(log);
    } else {
        renameFile(log, "recc_opt.log");
        enregistreGraphe(g, newFichier("recc.dot"));
    }
    
    terminateGraphe(g);
    terminateDesassembleur(desas); 
    
    terminateFichier(log);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_entropie(void){
    Fichier* log = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(log);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "entropie_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "entropie");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
        terminateDesassembleur(desas);    
    
    long diff = fequals(log, modele);
    if (diff == -1) {
        removeFichier(log);
    } else {
        renameFile(log, "entropie_opt.log");
        enregistreGraphe(g, newFichier("entropie.dot"));
    }
    
    terminateGraphe(g);
    terminateFichier(log);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_disas(void){
    Fichier* log = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(log);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "disas_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "disas");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
    
    terminateDesassembleur(desas);    
    
    long diff = fequals(log, modele);
    if (diff == -1) {
        removeFichier(log);
    } else {
        renameFile(log, "disas_opt.log");
        enregistreGraphe(g, newFichier("disas.dot"));
    }
    terminateGraphe(g);
    terminateFichier(log);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_handbrake(void){
    Fichier* log = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(log);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "handbrake_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "handbrake");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
   
    terminateDesassembleur(desas);    
    
    long diff = fequals(log, modele);
    if (diff == -1) {
        removeFichier(log);
    } else {
        renameFile(log, "handbrake_opt.log");
        enregistreGraphe(g, newFichier("handbrake.dot"));
    }
    terminateGraphe(g);
    terminateFichier(log);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}