#include "LoaderMacH.h"

unsigned long loaderMach(void* debutReel, DISASM* prog) {
    unsigned long addr = (unsigned long) debutReel;
    
    struct fat_header* fat = (struct fat_header*) addr;
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
    unsigned long debutVirtuel;
    unsigned long taille;
    while (!fini) {
        struct load_command *lc = (struct load_command *) addr;
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *sc = (struct segment_command_64 *) addr;
            unsigned long addrSec = addr + sizeof (struct segment_command_64);
            if (strcmp(sc->segname, "__TEXT") == 0) {
                while (!fini) { // il y a forcement un __text dans ce cas
                    struct section_64* sec = (struct section_64*) addrSec;
                    addrSec += sizeof (struct section_64);
                    if (strcmp(sec->sectname, "__text") == 0) {
                        fini = 1;
                        debutVirtuel = 0x100000000 + sec->offset; // /*+ sizeof (struct mach_header_64)*/ + (unsigned long) debutReel;
                        printf("début du bloc : 0x%lx\n", debutVirtuel);
                        taille = sec->size;
                        printf("taille du bloc : 0x%lx\n", taille);
                    }
                }
            }
        }
        //avance a la prochaine load_command
        addr += lc->cmdsize;
    }
    
    //===============================================================
    
    unsigned long pev;
    addr = (unsigned long) debutReel + sizeof (struct mach_header_64);
    fini = 0;
    while (!fini) {
        struct load_command *lc = (struct load_command *) addr;
        if (lc->cmd == LC_UNIXTHREAD) {
            struct lecteurRegistre* t = (struct lecteurRegistre*) addr;
            printf("point d'entrée : 0x%llx\n", t->state.uts.ts64.__rip);
            pev = t->state.uts.ts64.__rip;
            fini = 1;
        }
        addr += lc->cmdsize;
    }
    
    
    unsigned long per = pev - 0x100000000 + (unsigned long) debutReel;
    
    prog->EIP = (UIntPtr) per;
    prog->Archi = ARCHI_PROC;
    prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    prog->VirtualAddr = pev;
    prog->SecurityBlock = (unsigned int) (taille - (pev - debutVirtuel));

    printf("\nDone.\n\n");
    
    
    

    return debutVirtuel;
}