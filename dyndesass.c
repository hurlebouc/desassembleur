#include "dyndesass.h"

int placeRegistreInListe(char* chaine, LinkedList* listeRegistreAppel) {
    int i = 0;
    int fin = 0;
    int taille = (int) listeRegistreAppel->longueur;
    LinkedList* position = listeRegistreAppel;

    while (!fin && i < taille) {
        char* stringValue = position->valeur;
        if (strcmp(stringValue, chaine) == 0) {
            fin = 1;
        } else {
            i++;
            position = position->suiv;
        }
    }
    if (i == taille) {
        addLastLL(listeRegistreAppel, chaine);
    }
    return i;
}

/**
 * Cette fonction desassemble dynamiquement un binaire.
 * Par convention, le point d entree virtuel de _main sera en 0x100000000
 * Celui reel (EIP) devra être initialise en tenant compte du header
 * Le bloc de securite sera aussi initialise.
 * 
 * Tous les autres champs devront etre initialises a zero
 * 
 * On concidere pour le moment que seul un registre (rbp) fait office de pile
 * d appel.
 * 
 * On ce conciderera que les saut inconditionnels (sinon, il faut étudier les registres d etat)
 * 
 * @param prog
 */

void desassemblage_dynamique(DISASM* prog) {
    
    LinkedList* pileAppel = newLinkedList();
    int erreur = 0;
    void* finProg = prog->EIP+prog->SecurityBlock;
    int len;

    while (!erreur) {
        prog->SecurityBlock = finProg - prog->EIP;
        len = Disasm(prog);
        if (len == UNKNOWN_OPCODE) {
            erreur = 1;
            printf("Code inconnu\n");
        } else if (len == OUT_OF_BLOCK) {
            erreur = 1;
            printf("Fin du bloc");
        } else {
            void* adresseIni = prog->VirtualAddr;
            printf("0x%lx \t %s \t (0x%lx)\n", adresseIni, prog->CompleteInstr, prog->Instruction.AddrValue);
            void* pAdress = prog->Instruction.AddrValue;
            void* IP = adresseIni + len;
            if (pAdress != 0) { // elimine le cas ou on est pas dans une branche
                switch (prog->Instruction.BranchType) {
                    
                    case CallType:
                        addFirstLL(pileAppel, IP); // on empile
                        prog->VirtualAddr = prog->Instruction.AddrValue;
                        prog->EIP += prog->Instruction.AddrValue - (long) adresseIni;
                        break;

                    case JmpType:
                        prog->VirtualAddr = prog->Instruction.AddrValue;
                        prog->EIP += prog->Instruction.AddrValue - (long) adresseIni;
                        break;
                        
                    case RetType:
                        prog->VirtualAddr = pileAppel->valeur;
                        removeFirstLL(pileAppel);
                        prog->EIP += prog->Instruction.AddrValue - (long) adresseIni;
                        break;
                }

            } else {
                prog->VirtualAddr += len;
                prog->EIP += len;
            }
        }
    }
}