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
    strcat(chemin_modele, "mac/recc.dot");
    Fichier* modele = newFichier(chemin_modele);
    
    char chemin_bin[FILENAME_MAX];
    strcpy(chemin_bin, TESTS); 
    strcat(chemin_bin, "mac/recc");
    Fichier* binaire = newFichier(chemin_bin);
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_entropie(void){
        
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
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_disas(void){
        
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
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_handbrake(void){
        
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
    
    enregistrerCFG(binaire, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    terminateFichier(temp);
    terminateFichier(modele);
    terminateFichier(binaire);
    CU_ASSERT_EQUAL(diff, -1);
}










