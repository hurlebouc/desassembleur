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
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "recc.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "recc");
    Fichier* binaire = newFichier(chemin_bin);
    
//    Desasembleur* desas = newDesassembleur(NULL);
//    load(desas, binaire);
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_entropie(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "entropie.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "entropie");
    Fichier* binaire = newFichier(chemin_bin);
    
//    Desasembleur* desas = newDesassembleur(NULL);
//    load(desas, binaire);
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
//    terminateDesassembleur(desas);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_disas(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "disas.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "disas");
    Fichier* binaire = newFichier(chemin_bin);
    
//    Desasembleur* desas = newDesassembleur(NULL);
//    load(desas, binaire);
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
//    terminateDesassembleur(desas);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_handbrake(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    
    char chemin_modele[FILENAME_MAX];
    strcpy(chemin_modele, TESTS); 
    strcat(chemin_modele, "handbrake.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "handbrake");
    Fichier* binaire = newFichier(chemin_bin);
    
//    Desasembleur* desas = newDesassembleur(NULL);
//    load(desas, binaire);
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
//    terminateDesassembleur(desas);
    CU_ASSERT_EQUAL(diff, -1);
}










