#include <stdio.h>
#include <expat.h>

#include "macro.h"
#include "Fichier.h"
#include "desassembleur.h"
#include "loader.h"
#include "commandes.h"
#include "instruction.h"
#include "liste_instr.h"

char* ROOT;

int SYS = DESASSEMBLEUR_MAC;

int main(int argc, char* argv []) {
        
    ROOT = argv[0];
    
    unsigned int n = 0xc00a;
//    int* p = &n;
//    printf("p = %lx\n", p);
//    printf("p + 1 = %lx\n", p+1);
//    p = p+1;
//    printf("p + 1 = %lx\n", p);
    unsigned char i = *( ((char*) &n) + 1 );
//    unsigned char i = *( (char*) p );
    printf("i = %d\n", i);
    
    int** tab = malloc(sizeof(void*)*10);
    tab[2] = malloc(sizeof(int));
    *tab[2] = 4;
    free(tab[2]);
    if (tab[2] == NULL) {
        printf("NULL\n");
    } else {
        printf("%d\n", *tab[2]);
    }
    
    printf("done.\n");
    return 0;
}