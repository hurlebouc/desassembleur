//
//  testDyndesass.c
//  desassembleur
//
//  Created by Hubert Godfroy on 04/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#include "testDyndesass.h"

void cfg_recc(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    Fichier* modele = newFichier("../../../../tests/recc.dot");
    Fichier* binaire = newFichier("../../../../tests/recc");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    enregistrerCFG(desas, temp);
    
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
    Fichier* modele = newFichier("../../../../tests/entropie.dot");
    Fichier* binaire = newFichier("../../../../tests/entropie");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    enregistrerCFG(desas, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_disas(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    Fichier* modele = newFichier("../../../../tests/disas.dot");
    Fichier* binaire = newFichier("../../../../tests/disas");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    enregistrerCFG(desas, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    CU_ASSERT_EQUAL(diff, -1);
}

void cfg_handbrake(void){
        
    char chemintmp[L_tmpnam];
    tmpnam(chemintmp);
    Fichier* temp = newFichier(chemintmp);
    Fichier* modele = newFichier("../../../../tests/handbrake.dot");
    Fichier* binaire = newFichier("../../../../tests/handbrake");
    
    desasembleur* desas = newDesassembleur(NULL);
    load(desas, binaire);
    
    enregistrerCFG(desas, temp);
    
    long diff = fequals(temp, modele);
    if (diff == -1) {
        removeFichier(temp);
    }
    CU_ASSERT_EQUAL(diff, -1);
}










