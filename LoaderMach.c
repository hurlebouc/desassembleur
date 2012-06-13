#include "LoaderMach.h"
#include "_macro_Build.h"

static void initialiserFlags(Processeur* proc,uint64_t rflags){
    _CF = rflags % (CF_POWER*2);
    _PF = rflags % (PF_POWER*2);
    _AF = rflags % (AF_POWER*2);
    _ZF = rflags % (ZF_POWER*2);
    _SF = rflags % (SF_POWER*2);
    _TF = rflags % (TF_POWER*2);
    _IF = rflags % (IF_POWER*2);
    _DF = rflags % (DF_POWER*2);
    _OF = rflags % (OF_POWER*2);
}

static void initialiseRegistre(Processeur* proc, struct x86_thread_state state){
    setRegVal(_CS, state.uts.ts64.__cs);
    setRegVal(_FS, state.uts.ts64.__fs);
    setRegVal(_GS, state.uts.ts64.__gs);
    setRegVal(_R10, state.uts.ts64.__r10);
    setRegVal(_R11, state.uts.ts64.__r11);
    setRegVal(_R12, state.uts.ts64.__r12);
    setRegVal(_R13, state.uts.ts64.__r13);
    setRegVal(_R14, state.uts.ts64.__r14);
    setRegVal(_R15, state.uts.ts64.__r15);
    setRegVal(_R8, state.uts.ts64.__r8);
    setRegVal(_R9, state.uts.ts64.__r9);
    setRegVal(_RAX, state.uts.ts64.__rax);
    setRegVal(_RBP, state.uts.ts64.__rbp);
    setRegVal(_RBX, state.uts.ts64.__rbx);
    setRegVal(_RCX, state.uts.ts64.__rcx);
    setRegVal(_RDI, state.uts.ts64.__rdi);
    setRegVal(_RDX, state.uts.ts64.__rdx);
    setRegVal(_RFLAGS, state.uts.ts64.__rflags);
    setRegVal(_RIP, state.uts.ts64.__rip);
    setRegVal(_RSI, state.uts.ts64.__rsi);
    setRegVal(_RSP, state.uts.ts64.__rsp);
    initialiserFlags(proc, getRegVal(_RFLAGS));
}

void loaderMach(Desasembleur* desas, Fichier* fichier) {
    
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
            pev = t->state.uts.ts64.__rip;
            initialiseRegistre(desas->proc, t->state);
            fini = 1;
        }
        addr += lc->cmdsize;
    }
    
    
    unsigned long per = ((unsigned long) debutReel) + pev - (debutVirtuel - section->offset);
    
    prog->EIP = (UIntPtr) per;
    prog->Archi = 64;
    prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    prog->VirtualAddr = pev;
    prog->SecurityBlock = (unsigned int) (taille - (pev - debutVirtuel));

//    printf("\nDone.\n\n");
    close(fd);
    return;
}