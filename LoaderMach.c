#include "LoaderMacH.h"

void* loaderMach(void* debut) {
    char *addr = debut;


    // The first bytes of a Mach-O file comprise its header
    struct mach_header_64 *mh = (struct mach_header_64 *) addr;

    // Load commands follow the header
    addr += sizeof (struct mach_header_64);

    printf("There are %d load commands\n", mh->ncmds);

    int fini = 0;
    void* pe;
    while (!fini) {
        struct load_command *lc = (struct load_command *) addr;
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *sc = (struct segment_command_64 *) lc;
            void* addrSec = addr + sizeof (struct segment_command_64);
            if (strcmp(sc->segname, "__TEXT") == 0) {
                while (!fini) { // il y a forcement un __text dans ce cas
                    struct section_64 *sec = addrSec;
                    addrSec += sizeof (struct section_64);
                    if (strcmp(sec->sectname, "__text") == 0) {
                        printf("%d\n", sec->offset);
                        fini = 1;
                        pe = sec->offset /*+ sizeof (struct mach_header_64)*/ + debut;
                    }
                }
            }
        }
        //avance a la prochaine load_command
        addr += lc->cmdsize;
    }

    printf("\nDone.\n");

    return pe;
}