#include "loaderElf.h"

void loaderElf(desasembleur* desas, Fichier* fichier) {


    char* chemin = fichier->chemin;
    int fd = open(chemin, O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    /*============================= chargement en mémoire====================*/
    //    void* debutReel = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
    void* debutReel = mmap(0, size, PROT_READ | PROT_WRITE, 0 | MAP_PRIVATE, fd, 0);

    DISASM* prog = desas->disasm;



    unsigned long debutVirtuel = 0;
    Elf32_Ehdr *header = (Elf32_Ehdr *) debutReel; //les premier bits du elf contiennent un header général
    unsigned long per = (unsigned long) debutReel;
    printf("debut réel %lx \n", debutReel);
    if (header->e_type != ET_EXEC) {//vérifie que c'est bien un fichier executable pure
        printf("Ce fichier n'est pas un executable\n");
    } else {
        //comme c'est un executable, il y a forcement un tableau de header de section que l'on va examiner
        Elf32_Shdr *shdr = (Elf32_Shdr *) (debutReel + header->e_shoff);


        // il y a plusieurs string table, et celle qui comprend les noms des sections est la suivante
        //printf("index de strtable : %d\n", header->e_shstrndx);
        //on recupere son adresse dans premierchar
        char* premierchar = (char *) (debutReel + shdr[header->e_shstrndx].sh_offset);
        int length = (int) (shdr[header->e_shstrndx].sh_size) / sizeof (char);
        //printf("length : %d\n", length);

        //on va parcourir tout les header de section jusqu'a tomber sur celui de la section
        // .text qui contient le code qui va être executé
        int i = 0;
        for (i = 0; i < header->e_shnum; i++) {
            char* name = malloc(sizeof (char) * 5);
            //printf("le shname : %d\n", shdr[i].sh_name);
            int k = 0;
            for (k = 0; k < 5; k++) {
                name[k] = premierchar[shdr[i].sh_name + k];
            }
            //printf("le vrai nom %s\n", name);
            //c'est ici qu'on compare le nom de section
            if (strcmp(name, ".text") == 0) {
                //enregistre sa localisation dans pe
                per = debutReel + shdr[i].sh_offset;

                printf("offset du .text : %lx\n", shdr[i].sh_offset);

                debutVirtuel = 0x1000000 + shdr[i].sh_offset;

                prog->EIP = (UIntPtr) per;
                printf("EIP: %lx \n", prog->EIP);
                //l'emplacement du point d'entrée dans le buffer
                prog->Archi = 32;
                prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
                prog->VirtualAddr = debutVirtuel;
                printf("debutVirtuel: %lx \n", debutVirtuel);
                //trouve l'adresse du _start et non du main au sein de la section text
                //tester/regarder si le start est au début de la section text
                prog->SecurityBlock = (unsigned int) (shdr[i].sh_size);
                //taille qui lui reste a décompiler dans la section


            }
        }

        /*Cherche à récupérer l'adresse du main qui est le dernier element
         mis sur la pile d'appel par le dynamic linker lors de son
         initialisation (ie: avant de commencer, celui ci transmet sur la
         pile un vecteur d'information dont le dernier element est l'addresse
         est l'adresse du main*/

        DISASM retrieve;
        (void) memset(&retrieve, 0, sizeof (DISASM));


        retrieve.EIP = prog->EIP;
        retrieve.Archi = 32;
        retrieve.Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
        retrieve.SecurityBlock = prog->SecurityBlock;
        int lenght;
        unsigned long mainAddress;
        int halt = 0;
        int Error = 0;
        while ((!Error) && (0 < retrieve.SecurityBlock) && (!halt)) {
            
            lenght = Disasm(&retrieve);
            retrieve.SecurityBlock-=lenght;//fait un tour.
            
            if (lenght != UNKNOWN_OPCODE) {
                (void) puts(retrieve.CompleteInstr);
                retrieve.EIP = retrieve.EIP + (UIntPtr) lenght;
                printf("nombre de tour dans la boucle %lu \n", i);
                i++;
                //printf("SEcurity BLOCK %lu \n", retrieve.SecurityBlock);
                printf("mnémonic:%s!\n", retrieve.Instruction.Mnemonic);
                if (strcmp(retrieve.Instruction.Mnemonic, "push ") == 0) {
                    mainAddress =retrieve.Instruction.Immediat;
                    printf("ce qu'il y a dans immediat %lx \n", retrieve.Instruction.Immediat);
                    //printf("adresse supposé du main (offset): %lx \n", retrieve.Instruction.Immediat);
                }
                if (strcmp(retrieve.Instruction.Mnemonic, "hlt ") == 0) {
                    halt = 1;
                }
            } else {
                Error = 1;
            }
            printf("Halt: %lu, error: %lu, i: %lu \n", halt, Error, i);
                }
        printf("MAIN ADRESSE: est ce bien celui du push? %lx \n",mainAddress);
        mainAddress=mainAddress-header->e_entry;//offset dans la partie .text du main
        per=debutReel+mainAddress;
        printf("Est ce bien l'offset du main dans le fichier: %lx \n",mainAddress);
        per=(UIntPtr) per + mainAddress;
        prog->EIP = (UIntPtr) per;
        desas->debutVirtuel=debutVirtuel;
        //l'emplacement du point d'entrée dans le buffer
        prog->Archi = 32;
        prog->Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
        prog->VirtualAddr = debutVirtuel+mainAddress;
        //trouve l'adresse du _start et non du main au sein de la section text
        //tester/regarder si le start est au début de la section text
        //pe = debut de la section text
   
        printf("Taille du bloc sécurité: %lx \n",prog->SecurityBlock);
        //taille qui lui reste a décompiler dans la section

        printf("EIP: %lx \n", prog->EIP);
        printf("debutVirtuel: %lx \n", debutVirtuel);
    }




}




