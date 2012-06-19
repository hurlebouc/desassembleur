//
//  testDyndesass.c
//  desassembleur
//
//  Created by Hubert Godfroy on 04/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

extern char* TESTS;

#include "testDyndesass.h"
#include <stdio.h>

void cfg_recc(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logSimplifie = newFichier(CHEMIN_LOG_ASSEMBLAGE);
    
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/recc.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/recc");
    Fichier* binaire = newFichier(chemin_bin);
    
    enregistrerCFG(binaire, temp, MACHO_64);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
        removeFichier(logBuild);
        removeFichier(logSimplifie);
    } else {
        renameFile(logBuild, "recc_build.log");
        renameFile(logSimplifie, "recc_simplifie.log");
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    terminateFichier(logSimplifie);
    terminateFichier(logBuild);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_entropie(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logSimplifie = newFichier(CHEMIN_LOG_ASSEMBLAGE);
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/entropie.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/entropie");
    Fichier* binaire = newFichier(chemin_bin);
    
    enregistrerCFG(binaire, temp, MACHO_64);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
        removeFichier(logBuild);
        removeFichier(logSimplifie);
    } else {
        renameFile(logBuild, "entropie_build.log");
        renameFile(logSimplifie, "entropie_simplifie.log");
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    terminateFichier(logSimplifie);
    terminateFichier(logBuild);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_disas(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logSimplifie = newFichier(CHEMIN_LOG_ASSEMBLAGE);
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/disas.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/disas");
    Fichier* binaire = newFichier(chemin_bin);
    
    enregistrerCFG(binaire, temp, MACHO_64);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
        removeFichier(logBuild);
        removeFichier(logSimplifie);
    } else {
        renameFile(logBuild, "disas_build.log");
        renameFile(logSimplifie, "disas_simplifie.log");
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    terminateFichier(logSimplifie);
    terminateFichier(logBuild);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_handbrake(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logSimplifie = newFichier(CHEMIN_LOG_ASSEMBLAGE);
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "mac/handbrake.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/handbrake");
    Fichier* binaire = newFichier(chemin_bin);
    
    enregistrerCFG(binaire, temp, MACHO_64);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
        removeFichier(logBuild);
        removeFichier(logSimplifie);
    } else {
        renameFile(logBuild, "handbrake_build.log");
        renameFile(logSimplifie, "handbrake_simplifie.log");
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    terminateFichier(logSimplifie);
    terminateFichier(logBuild);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_prog_linux(void){
    Fichier* logBuild = newFichier(CHEMIN_LOG_FERMETURE);
    Fichier* logSimplifie = newFichier(CHEMIN_LOG_ASSEMBLAGE);
    
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "linux/prog.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "linux/prog");
    Fichier* binaire = newFichier(chemin_bin);
    
    
    enregistrerCFG(binaire, temp, ELF_32);
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
        removeFichier(logBuild);
        removeFichier(logSimplifie);
    } else {
        renameFile(logBuild, "linux_build.log");
        renameFile(logSimplifie, "linux_simplifie.log");
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    terminateFichier(logSimplifie);
    terminateFichier(logBuild);
    CU_ASSERT_EQUAL(diff, -1);
}










