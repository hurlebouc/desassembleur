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
    
    enum ValeurImmediat jkr = argc*293;
    
    test* t = init_test();
        
    printf("%d\n", do_test(t, 23));
    printf("%d\n", t->f(23));
    printf("%u\n", jkr);
        
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
    printf("done.\n");
    return 0;
}