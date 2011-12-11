#ifndef STRING
#define STRING
#include <string.h>
#endif

typedef struct LinkedList
{
    void* valeur;
    unsigned long longueur;
    struct LinkedList* prec;
    struct LinkedList* suiv;
    struct LinkedList* dernier;
}LinkedList;


LinkedList* newLinkedList();
unsigned long sizeLL(LinkedList* liste);
unsigned long sizeExtLL(LinkedList* liste);

void addLastLL(LinkedList* liste, void* element);
void addFirstLL(LinkedList* liste, void* element);
void addLL(LinkedList* liste, void* element, unsigned long index);
void addAllLL(LinkedList* recepteur, LinkedList* source);
void addAllLL2(LinkedList* recepteur, LinkedList* source);

void addLastCharLL(LinkedList* liste, char c);

void* getLastLL(LinkedList* liste);
void* getFirstLL(LinkedList* liste);
void* getLL(LinkedList* liste, unsigned long index);

char getCharLL(LinkedList* liste, unsigned long index);

void* removeLastLL(LinkedList* liste);
void* removeFirstLL(LinkedList* liste);
void* removeAtIndexLL(LinkedList* liste, unsigned long index);
void removeElementLL(LinkedList* liste, void* element);

unsigned long indexOfLL(LinkedList* liste, void* element);
long indexOfCharLL(LinkedList* liste, char element);

void terminateLinkedList(LinkedList* liste);
void terminateLinkedListTotal(LinkedList* liste);

void afficheLinkedList(LinkedList* liste, short tailleElement);


LinkedList* copieLinkedList(LinkedList* liste);
/*
 a utiliser avec beaucoup de précautions car cette foncion
 ne copie pas les objets pointés par le pointeur valeur
 */