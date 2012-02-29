#include "loaderElf.h"

void* loaderElf(void *debut) {
    void* pe = debut;
    Elf32_Ehdr *header = (Elf32_Ehdr *) debut;
    Elf32_Shdr *shdr = (Elf32_Shdr *) (debut + header->e_shoff);
    
    printf("index de strtable : %d\n", header->e_shstrndx);
    
    char* premierchar = (char *) (debut + shdr[header->e_shstrndx].sh_offset);
    int length = (int) (shdr[header->e_shstrndx].sh_size)/sizeof(char);
    printf("length : %d\n", length);
   
    int i = 0;
    for (i = 0; i < header->e_shnum; i++) {
        char* name = malloc(sizeof(char) * 5);
        printf("le shname : %d\n", shdr[i].sh_name);
        int k = 0;
        for(k = 0 ; k < 5 ; k++){
            name[k] = premierchar[shdr[i].sh_name + k];
        }
        printf("le vrai nom %s\n", name);
        if(strcmp(name, ".text")  == 0){
            pe += shdr[i].sh_offset;
            printf("offset du .text : %d\n", shdr[i].sh_offset);
        }
    }
    return pe;
}





