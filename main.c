#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BEA_ENGINE_STATIC /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL /* specify the usage of a stdcall version of BeaEngine */
#ifndef BEAENGINE
#define BEAENGINE
#include "BeaEngine.h"
#endif

#ifndef MACH
#define MACH
#include "LoaderMach.h"
#endif

//#include <mach-o/loader.h>

/**
 * Cette fonction desassemble statiquement un binaire.
 * 
 * Pour le moment, elle ne marche pas car le desassemeblage commence des le debut
 * du fichier sans tenir compte des headers.
 * 
 * De plus, une fois finie, cette fonction ne sera pas portable car elle dependra 
 * de la structure de binaire pour chaque OS (Mach-O par Mac, EFL pour Linux, ...)
 * 
 * ATTENTION : pour le moment, aucune borne n est implementee
 * (la terminaison n est pas verifiee)
 * 
 * utiliser #include <mach-o/loader.h>
 * 
 * @param chemin chemin du bianire
 */

void lireProg(char* chemin) {
    DISASM prog;
    (void) memset(&prog, 0, sizeof (DISASM));

    FILE* FileHandle = fopen(chemin, "rb");
    (void) fseek(FileHandle, 0, SEEK_END);
    long FileSize = ftell(FileHandle);
    (void) rewind(FileHandle);
    char* pBuffer = malloc(FileSize);
    (void) fread(pBuffer, FileSize, sizeof (char), FileHandle);
    (void) fclose(FileHandle);

    prog.EIP = pBuffer;
    prog.VirtualAddr = pBuffer;

    prog.Archi = ARCHI_PROC;
    prog.Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;

    int Error = 0;
    int len;
    int i = 0;

    while ((!Error) && (i < 80)) {
        len = Disasm(&prog);
        if (len != UNKNOWN_OPCODE) {
            printf("0x%lx \t %s \t (0x%lx)\n", prog.EIP, prog.CompleteInstr, prog.Instruction.AddrValue);
            prog.EIP += (UIntPtr) len;
            prog.VirtualAddr += len;
            i++;
        } else {
            Error = 1;
        }
    }
    free(pBuffer);
}

int main(int argc, char* argv []) {

    //lireProg("dist/Release/GNU-MacOSX/desassemblage");

    printf("\n ============================= \n");

    /* ============================= Init datas */
    DISASM MyDisasm;
    int len, i = 0;
    int Error = 0;

    /* ===================== display the version and revision used */
    (void) printf("Version : %s\n", BeaEngineVersion());
    (void) printf("Revision : %s\n", BeaEngineRevision());
    /* ============================= Init the Disasm structure (important !)*/
    (void) memset(&MyDisasm, 0, sizeof (DISASM));

    /* ============================= Init EIP */
    MyDisasm.EIP = &main;
    MyDisasm.Archi = ARCHI_PROC;
    MyDisasm.Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    MyDisasm.VirtualAddr = 0x100000000;
    /* ============================= Loop for Disasm */
    while ((!Error) && (i < 15)) {
        len = Disasm(&MyDisasm);
        if (len != UNKNOWN_OPCODE) {
            printf("0x%lx \t %s \t (0x%lx) \t \n",
                    MyDisasm.VirtualAddr,
                    MyDisasm.CompleteInstr,
                    MyDisasm.Instruction.AddrValue);
            MyDisasm.EIP += (UIntPtr) len;
            MyDisasm.VirtualAddr += len;
            i++;
        } else {
            Error = 1;
        }
    };

    printf("\n=========== fin 1 =========\n \n");

    int fd = open("dist/Debug_Mac/GNU-MacOSX/desassembleur-code", O_RDONLY);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    size_t size = stat_buf.st_size;
    void* debut = mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);

    void* pe = loaderMac(debut);


    MyDisasm.EIP = pe;
    MyDisasm.Archi = ARCHI_PROC;
    MyDisasm.Options = Tabulation + NasmSyntax + PrefixedNumeral + ShowSegmentRegs;
    MyDisasm.VirtualAddr = 0x100000000 + pe - debut;
    MyDisasm.SecurityBlock = 40000;
    desassemblage_dynamique(&MyDisasm);
    
    munmap(debut, size);
    close(fd);

    return 0;

}