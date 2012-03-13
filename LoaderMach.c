#include "LoaderMach.h"

static void initialiserFlags(Processeur* proc,uint64_t rflags){
    proc->_cf = rflags % 2;
    proc->_pf = rflags % 8;
    proc->_af = rflags % 32;
    proc->_zf = rflags % 128;
    proc->_sf = rflags % 256;
    proc->_tf = rflags % 512;
    proc->_if = rflags % 1024;
    proc->_df = rflags % 2048;
    proc->_of = rflags % 4096;
}

static void initialiseRegistre(Processeur* proc, struct x86_thread_state state){
    setValeur(proc->cs, state.uts.ts64.__cs);
    setValeur(proc->fs, state.uts.ts64.__fs);
    setValeur(proc->gs, state.uts.ts64.__gs);
    setValeur(proc->r10, state.uts.ts64.__r10);
    setValeur(proc->r11, state.uts.ts64.__r11);
    setValeur(proc->r12, state.uts.ts64.__r12);
    setValeur(proc->r13, state.uts.ts64.__r13);
    setValeur(proc->r14, state.uts.ts64.__r14);
    setValeur(proc->r15, state.uts.ts64.__r15);
    setValeur(proc->r8, state.uts.ts64.__r8);
    setValeur(proc->r9, state.uts.ts64.__r9);
    setValeur(proc->rax, state.uts.ts64.__rax);
    setValeur(proc->rbp, state.uts.ts64.__rbp);
    setValeur(proc->rbx, state.uts.ts64.__rbx);
    setValeur(proc->rcx, state.uts.ts64.__rcx);
    setValeur(proc->rdi, state.uts.ts64.__rdi);
    setValeur(proc->rdx, state.uts.ts64.__rdx);
    setValeur(proc->rflags, state.uts.ts64.__rflags);
    setValeur(proc->rip, state.uts.ts64.__rip);
    setValeur(proc->rsi, state.uts.ts64.__rsi);
    setValeur(proc->rsp, state.uts.ts64.__rsp);
    initialiserFlags(proc, getValeur(proc->rflags));
}

void loaderMach(desasembleur* desas, Fichier* fichier) {
    
    char* chemin = fichier->chemin;
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    /*============================= chargement en mémoire====================*/
    void* debutReel = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    
    DISASM* prog = desas->disasm;
    
    
    
    struct fat_header* fat = (struct fat_header*) debutReel;
    if (fat->magic == FAT_CIGAM) { // du coup ça m apprend que mon processeur est un little-endian
                                   // en effet, apres recherche, les intels sont little-endien, les motorola sont big-endian
        printf("c est un fat binary : je ne peux de le desassembler\n(dans un prochaine version sans doute...)\n");
        exit(EXIT_FAILURE);
    }
    
//    unsigned long addr = (unsigned long) debutReel;
//
//    // The first bytes of a Mach-O file comprise its header
////    struct mach_header_64 *mh = (struct mach_header_64 *) addr;
//
//    // Load commands follow the header
//    addr += sizeof (struct mach_header_64);
//
////    printf("There are %d load commands\n", mh->ncmds);
//
//    int fini = 0;
//    unsigned long debutVirtuel;
//    unsigned long taille;
//    while (!fini) {
//        struct load_command *lc = (struct load_command *) addr;
//        if (lc->cmd == LC_SEGMENT_64) {
//            struct segment_command_64 *sc = (struct segment_command_64 *) addr;
//            if (strcmp(sc->segname, "__TEXT") == 0) {
//                unsigned long addrSec = addr + sizeof (struct segment_command_64);
//                while (!fini) { // il y a forcement un __text dans ce cas
//                    struct section_64* sec = (struct section_64*) addrSec;
//                    addrSec += sizeof (struct section_64);
//                    if (strcmp(sec->sectname, "__text") == 0) {
//                        fini = 1;
//                        debutVirtuel = 0x100000000 + sec->offset; // /*+ sizeof (struct mach_header_64)*/ + (unsigned long) debutReel;
////                        printf("début du bloc : 0x%lx\n", debutVirtuel);
//                        taille = sec->size;
////                        printf("taille du bloc : 0x%lx\n", taille);
//                    }
//                }
//            }
//        }
//        //avance a la prochaine load_command
//        addr += lc->cmdsize;
//    }
    
    //===============================================================
    
    const struct section_64* section = getsectbynamefromheader_64(debutReel, "__TEXT", "__text");
    unsigned long debutVirtuel = section->addr;
    unsigned long taille = section->size;
    desas->debutVirtuel = debutVirtuel;
    
    unsigned long pev;
    unsigned long addr = (unsigned long) debutReel + sizeof (struct mach_header_64);
    int fini = 0;
    while (!fini) {
        struct load_command *lc = (struct load_command *) addr;
        if (lc->cmd == LC_UNIXTHREAD) {
            struct lecteurRegistre* t = (struct lecteurRegistre*) addr;
//            printf("point d'entrée : 0x%llx\n", t->state.uts.ts64.__rip);
            pev = t->state.uts.ts64.__rip;
            initialiseRegistre(desas->proc, t->state);
            fini = 1;
        }
        addr += lc->cmdsize;
    }
    
    
    unsigned long per = ((unsigned long) debutReel) + pev - (debutVirtuel - section->offset);
    
    prog->EIP = (UIntPtr) per;
    prog->Archi = ARCHI_PROC;
    prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    prog->VirtualAddr = pev;
    prog->SecurityBlock = (unsigned int) (taille - (pev - debutVirtuel));

//    printf("\nDone.\n\n");
    close(fd);
    return;
}