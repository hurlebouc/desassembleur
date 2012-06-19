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
    Fichier* logOpt = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(logOpt);
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/recc_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/recc");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, MACHO_64);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
       
    
    long diff = fequals(logOpt, modele);
    if (diff == -1) {
        removeFichier(logOpt);
        removeFichier(logBuild);
    } else {
        renameFile(logOpt, "recc_opt.log");
        enregistreGraphe(g, newFichier("recc.dot"));
        renameFile(logBuild, "recc_build.log");
    }
    
    terminateGraphe(g);
    terminateDesassembleur(desas); 
    terminateFichier(logBuild);
    terminateFichier(logOpt);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_entropie(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logOpt = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(logOpt);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/entropie_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/entropie");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, MACHO_64);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
        terminateDesassembleur(desas);    
    
    long diff = fequals(logOpt, modele);
    if (diff == -1) {
        removeFichier(logOpt);
        removeFichier(logBuild);
    } else {
        renameFile(logOpt, "entropie_opt.log");
        enregistreGraphe(g, newFichier("entropie.dot"));
        renameFile(logBuild, "entropie_build.log");
    }
    
    terminateGraphe(g);
    terminateFichier(logOpt);
    terminateFichier(logBuild);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_disas(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logOpt = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(logOpt);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/disas_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/disas");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, MACHO_64);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
    
    terminateDesassembleur(desas);    
    
    long diff = fequals(logOpt, modele);
    if (diff == -1) {
        removeFichier(logOpt);
        removeFichier(logBuild);
    } else {
        renameFile(logOpt, "disas_opt.log");
        enregistreGraphe(g, newFichier("disas.dot"));
        renameFile(logBuild, "disas_build.log");
    }
    terminateGraphe(g);
    terminateFichier(logOpt);
    terminateFichier(logBuild);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void optpool_handbrake(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logOpt = newFichier(CHEMIN_LOG_OPTIMISATION);
    cleanFile(logOpt);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/handbrake_opt.log");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/handbrake");
    Fichier* binaire = newFichier(chemin_bin);
    
    Desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire, MACHO_64);
    Graphe* g = ControleFlow_entier(desas);
    g->_immediat = EST_ASSEMBLE;
    optimizePool2(g, desas->proc);
   
    terminateDesassembleur(desas);    
    
    long diff = fequals(logOpt, modele);
    if (diff == -1) {
        removeFichier(logOpt);
        removeFichier(logBuild);
    } else {
        renameFile(logOpt, "handbrake_opt.log");
        enregistreGraphe(g, newFichier("handbrake.dot"));
        renameFile(logBuild, "handbrake_build.log");
    }
    terminateGraphe(g);
    terminateFichier(logOpt);
    terminateFichier(logBuild);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}