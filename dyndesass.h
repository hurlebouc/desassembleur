/*! 
 * @file dyndesass.h
 */

#ifndef desassembleur_dyndesass_h
#define desassembleur_dyndesass_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
        
#include "macro.h"
#include "LinkedList.h"
#include "Graphe.h"
#include "desassembleur.h"
#include "Fichier.h"
    
    extern char* ROOT;
    
    void desassemblage_inconditionnel(DISASM* prog);
    
    Graphe* buildGraphe(Desasembleur* desas, Graphe* pi[]);
    Graphe* simplifieGraphe(Desasembleur* desas, Graphe* g);
    
    Graphe* ControleFlow_entier(Desasembleur* desas);
    Graphe* ControleFlow_simplifie(Desasembleur* desas);
    
    void afficheGraphe(Graphe* g);
    void afficherPI(Graphe* pi, unsigned long taille);
    void afficheCrible(int* crible, unsigned long taille, unsigned long pev);
    
    void enregistreGraphe(Graphe* g, Fichier* tmp);
    
#ifdef __cplusplus
}
#endif

#endif
