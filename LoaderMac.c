#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach-o/loader.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int loaderMac(char* chemin) {
    int fd;
    struct stat stat_buf;
    size_t size;

    char *addr = NULL;
    struct mach_header_64 *mh;
    struct load_command *lc;
    struct segment_command_64 *sc;

    // Open the file and get its size
    fd = open(chemin, O_RDONLY);
    fstat(fd, &stat_buf);
    size = stat_buf.st_size;

    // Map the file to memory
    addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    void* debut = addr;


    // The first bytes of a Mach-O file comprise its header
    mh = (struct mach_header_64 *) addr;

    // Load commands follow the header
    addr += sizeof (struct mach_header_64);

    printf("There are %d load commands\n", mh->ncmds);

    int fini = 0;
    unsigned long pe;
    while (!fini) {
        lc = (struct load_command *) addr;
        if (lc->cmd == LC_SEGMENT_64) {
            sc = (struct segment_command_64 *) lc;
            void* addrSec = addr + sizeof (segment_command_64);
            if (strcmp(sc->segname, "__TEXT") == 0) {
                int fini = 0;
                while (!fini) { // il y a forcement un __text dans ce cas
                    struct section_64 *sec = addrSec;
                    addrSec += sizeof (section_64);
                    if (strcmp(sec->sectname, "__text") == 0) {
                        fini = 1;
                        pe = sec->offset + sizeof (mach_header_64) + debut;
                    }
                }
            }
            /*
                        printf("Segment %s\n\t"
                                "vmaddr 0x%llx\n\t"
                                "vmsize 0x%llx\n\t"
                                "fileoff %llu\n\t"
                                "filesize %llu\n",
                                sc->segname,
                                sc->vmaddr,
                                sc->vmsize,
                                sc->fileoff,
                                sc->filesize);
             */
        }

        // Advance to the next load command    
        addr += lc->cmdsize;
    }

    printf("\nDone.\n");

    munmap(addr, size);
    close(fd);

    return 0;
}