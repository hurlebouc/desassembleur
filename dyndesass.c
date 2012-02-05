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
 * On ce conciderera que les saut inconditionnels (sinon, il faut étudier les registres d etat)
 * 
 * @param prog
 */

void desassemblage_inconditionnel(DISASM* prog) {

    LinkedList* pileAppel = newLinkedList();
    int erreur = 0;
    unsigned long debut = prog->EIP;
    unsigned long finProg = prog->EIP + prog->SecurityBlock;
    int len;

    while (!erreur) {
        prog->SecurityBlock = (unsigned int) (finProg - prog->EIP);
        len = Disasm(prog);
        //printf("SecurityBlock = %d et len = %d \n", prog->SecurityBlock, len);
        if (len == UNKNOWN_OPCODE) {
            erreur = 1;
            printf("Code inconnu\n");
            printf("arrete apres %ld octets\n", prog->EIP-debut);
        } else if (len == OUT_OF_BLOCK) {
            erreur = 1;
            printf("Fin du bloc\n");
            printf("arrete apres %ld octets\n", prog->EIP-debut);
        } else {
            unsigned long adresseIni = prog->VirtualAddr;
            printf("(0x%lx) \t 0x%lx \t %s \t (0x%lx)\n", prog->EIP, adresseIni, prog->CompleteInstr, prog->Instruction.AddrValue);
            unsigned long IP = adresseIni + len;
            switch (prog->Instruction.BranchType) {

                case CallType:
                    addFirstLL(pileAppel, (void *) IP); // on empile
                    prog->VirtualAddr = prog->Instruction.AddrValue;
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                case JmpType:
                    prog->VirtualAddr = prog->Instruction.AddrValue;
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                case RetType:
                    prog->VirtualAddr = (unsigned long) pileAppel->valeur;
                    removeFirstLL(pileAppel); // on depile
                    prog->EIP += prog->VirtualAddr - (long) adresseIni;
                    break;

                default:
                    prog->VirtualAddr += len;
                    prog->EIP += len;
            }
            if (prog->EIP >= finProg) {
                printf("fin de la lecture");
                printf("arrete apres %ld octets\n", prog->EIP-debut);
                erreur = 1;
            }
        }
    }
}

/**
 * pi est un tebleau de Graphes.
 */

void reperageJump(DISASM* prog, Graphe pi[]){
    int stop = 0;
    unsigned long debut = prog->EIP;
    unsigned long fin = prog->SecurityBlock + debut;
    unsigned long debutVirtuel = prog->VirtualAddr;
    while (!stop) {
        prog->SecurityBlock = (int) (fin - prog->EIP);
        int len = Disasm(prog);
        switch (len) {
            case UNKNOWN_OPCODE:
                printf("opcode inconnu\n");
                exit(EXIT_FAILURE);
                break;
                
            case OUT_OF_BLOCK:
                printf("depassement de bloc\n");
                stop = 1;
                break;
                
            default:
                if (prog->Instruction.BranchType!=0) {
                    int brancheType = prog->Instruction.BranchType;
                    Graphe gIni = pi[prog->VirtualAddr - debutVirtuel];
                    Graphe gTarget = pi[prog->Instruction.AddrValue - debutVirtuel];
                    Graphe gSuivant = pi[prog->VirtualAddr - debutVirtuel + len];
                    gIni.interet = 1;
                    gTarget.interet = 1;
                    
                    if (brancheType == JmpType) {
                        LinkedList* listeFils = newLinkedList();
                        addFirstLL(listeFils, (void*) prog->Instruction.AddrValue); // on ajoute la cible du jump
                        gIni.listeFils = listeFils;
                        addFirstLL(gTarget.listePeres, (void*) prog->VirtualAddr); // on ajoute l'adresse local aux peres de la cible
                        
                    } else if(prog->Instruction.BranchType != CallType && prog->Instruction.BranchType != RetType){
                        gSuivant.interet = 1;
                        LinkedList* listeFils = newLinkedList();
                        addFirstLL(listeFils, (void*) prog->Instruction.AddrValue); // on ajoute la cible du jump
                        addFirstLL(listeFils, (void*) (prog->VirtualAddr + len)); // on ajoute l'instruction suivante
                        gIni.listeFils = listeFils;
                        addFirstLL(gTarget.listePeres, (void*) prog->VirtualAddr); // on ajoute l'adresse local aux peres de la cible
                        addFirstLL(gSuivant.listePeres, (void*) prog->VirtualAddr); // on ajoute l'adresse local aux peres de l'instruction suivante
                    }
                    pi[prog->VirtualAddr - debutVirtuel] = gIni;
                    pi[prog->Instruction.AddrValue - debutVirtuel] = gTarget;
                    pi[prog->VirtualAddr - debutVirtuel + len] = gSuivant;
                }
                
                
                prog->VirtualAddr += len;
                prog->EIP += len;
                if (prog->EIP >= fin) {
                    printf("fin de la lecture");
                    stop = 1;
                }
                break;
        }
    }
}

void reperageAppels(DISASM* prog, Graphe pi[]){
    
}

/**
 * Cette fonction assemble les elements du tableau
 * pour en faire un seul graphe
 */

Graphe* assembleGraphe(DISASM* prog, Graphe pi[]){
    
}

Graphe* ControleFlow(DISASM* prog){
    unsigned long taille = prog->SecurityBlock;
    unsigned long debut = prog->EIP;
    unsigned long virtualAddr = prog->VirtualAddr;
    Graphe* pi = calloc(sizeof(Graphe),prog->SecurityBlock);
    reperageJump(prog, pi);
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    reperageAppels(prog, pi);
    
    prog->EIP = debut;
    prog->SecurityBlock = taille;
    prog->VirtualAddr = virtualAddr;
    return assembleGraphe(prog, pi);
}

