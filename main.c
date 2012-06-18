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

int main(int argc, char* argv []) {
        
    ROOT = argv[0];
    char c = '0';
    printf("ASCII %c : %d\n", c, c);
    return 0;
}