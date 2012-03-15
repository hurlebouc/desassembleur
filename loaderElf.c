#include "loaderElf.h"

unsigned long loaderElf(desasembleur* desas, Fichier* fichier) {
    
    
    char* chemin = fichier->chemin;
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    /*============================= chargement en mémoire====================*/
//    void* debutReel = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    void* debutReel = mmap(0, size, PROT_READ | PROT_WRITE, 0 | MAP_PRIVATE, fd, 0);
    
    DISASM* prog = desas->disasm;
    
    
    unsigned long per = (unsigned long) debutReel;
    unsigned long debutVirtuel = 0;
    Elf32_Ehdr *header = (Elf32_Ehdr *) debutReel; //les premier bits du elf contiennent un header général
    if (header->e_type != ET_EXEC) {//vérifie que c'est bien un fichier executable pure
        printf("Ce fichier n'est pas un executable\n");
    } else {
        //comme c'est un executable, il y a forcement un tableau de header de section que l'on va examiner
        Elf32_Shdr *shdr = (Elf32_Shdr *) (debutReel + header->e_shoff);
        
        
        // il y a plusieurs string table, et celle qui comprend les noms des sections est la suivante
        printf("index de strtable : %d\n", header->e_shstrndx);
        //on recupere son adresse dans premierchar
        char* premierchar = (char *) (debutReel + shdr[header->e_shstrndx].sh_offset);
        int length = (int) (shdr[header->e_shstrndx].sh_size) / sizeof (char);
        printf("length : %d\n", length);
        
        //on va parcourir tout les header de section jusqu'a tomber sur celui de la section
        // .text qui contient le code qui va être executé
        int i = 0;
        for (i = 0; i < header->e_shnum; i++) {
            char* name = malloc(sizeof (char) * 5);
            printf("le shname : %d\n", shdr[i].sh_name);
            int k = 0;
            for (k = 0; k < 5; k++) {
                name[k] = premierchar[shdr[i].sh_name + k];
            }
            printf("le vrai nom %s\n", name);
            //c'est ici qu'on compare le nom de section
            if (strcmp(name, ".text") == 0) {
                //enregistre sa localisation dans pe
                per += shdr[i].sh_offset;
                
                printf("offset du .text : %d\n", shdr[i].sh_offset);
                
                debutVirtuel=0x10000000+shdr[i].sh_offset;
                prog->EIP = (UIntPtr) per;
                //l'emplacement du point d'entrée dans le buffer
                prog->Archi = 32;
                prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
                prog->VirtualAddr = debutVirtuel ;
                //trouve l'adresse du _start et non du main au sein de la section text
                //tester/regarder si le start est au début de la section text
                prog->SecurityBlock = (unsigned int) (shdr[i].sh_size);
                //taille qui lui reste a décompiler dans la section
                
                
            }
        }
        printf("Debutvirtuel: %lx\n",debutVirtuel);
    }
    return debutVirtuel;
}





