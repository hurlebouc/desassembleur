#include "LoaderMacH.h"

void* loaderMach(void* debut, DISASM* prog) {
    char *addr = debut;
    
    struct fat_header* fat = addr;
    if (fat->magic == FAT_CIGAM) { // du coup ça m apprend que mon processeur est un little-endian
                                   // en effet, apres recherche, les intels sont little-endien, les motorola sont big-endian
        printf("c est un fat binary : je ne peux de le desassembler\n(dans un prochaine version sans doute...)\n");
        exit(EXIT_FAILURE);
    }


    // The first bytes of a Mach-O file comprise its header
    struct mach_header_64 *mh = (struct mach_header_64 *) addr;

    // Load commands follow the header
    addr += sizeof (struct mach_header_64);

    printf("There are %d load commands\n", mh->ncmds);

    int fini = 0;
    void* pe;
    unsigned long taille;
    while (!fini) {
        struct load_command *lc = (struct load_command *) addr;
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *sc = (struct segment_command_64 *) addr;
            void* addrSec = addr + sizeof (struct segment_command_64);
            if (strcmp(sc->segname, "__TEXT") == 0) {
                while (!fini) { // il y a forcement un __text dans ce cas
                    struct section_64 *sec = addrSec;
                    addrSec += sizeof (struct section_64);
                    if (strcmp(sec->sectname, "__text") == 0) {
                        printf("point d entree : 0x%x\n", sec->offset);
                        fini = 1;
                        pe = sec->offset /*+ sizeof (struct mach_header_64)*/ + debut;
                        taille = sec->size;
                        printf("taille du bloc : 0x%lx\n", taille);
                    }
                }
            }
        }
        //avance a la prochaine load_command
        addr += lc->cmdsize;
    }
    
    prog->EIP = (UIntPtr) pe;
    prog->Archi = ARCHI_PROC;
    prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    prog->VirtualAddr = 0x100000000 + pe - debut;
    prog->SecurityBlock = (unsigned int) taille;

    printf("\nDone.\n");

    return pe;
}