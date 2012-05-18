#include "LinkedList.h"

/* -------------------------------------------------------------------- *
 *                          LINKEDLIST                                  *
 * -------------------------------------------------------------------- */

LinkedList* newLinkedList(void) {
    LinkedList* res = malloc(sizeof (LinkedList));
    res->valeur = NULL;
    res->prec = NULL;
    res->suiv = NULL;
    res->dernier = res;
    res->longueur = 0;
    return res;
}

LinkedList* newLinkedListCopy(LinkedList* l){
    if (l==NULL) {
        return NULL;
    }
    LinkedList* res = newLinkedList();
    LinkedList* tete = l;
    while (tete != NULL) {
        addLastLL(res, tete->valeur);
        tete = tete->suiv;
    }
    return res;
}

unsigned long sizeExtLL(LinkedList* liste) {
    if (liste->valeur == NULL) {
        return 0;
    }
    LinkedList* position = liste;
    unsigned long res = 0;
    while (position != NULL) {
        res++;
        position = position->suiv;
    }
    return res;
}

unsigned long sizeLL(LinkedList* liste) {
    return liste->longueur;
}



void addLastLL(LinkedList* liste, void* element) {
    if (sizeLL(liste) == 0) {
        liste->longueur++;
        liste->valeur = element;
    } else {
        liste->longueur++;
        LinkedList* nouv = newLinkedList();
        nouv->valeur = element;
        (liste->dernier)->suiv = nouv;
        nouv->prec = liste->dernier;
        liste->dernier = nouv;
    }
}

void addFirstLL(LinkedList* liste, void* element) {
    if (sizeLL(liste) == 0) {
        liste->longueur++;
        liste->valeur = element;
    } else {
        liste->longueur++;
        LinkedList* nouv = newLinkedList();
        nouv->valeur = liste->valeur;
        nouv->suiv = liste->suiv;
        nouv->prec = liste;
        if (liste->suiv != NULL) {
            (liste->suiv)->prec = nouv;
        }
        liste->valeur = element;
        liste->suiv = nouv;
        if (nouv->suiv == NULL) {
            liste->dernier = nouv;
        }
    }
}

void addLL(LinkedList* liste, void* element, unsigned long index) {
    if (index == 0) {
        addFirstLL(liste, element);
        return;
    }
    unsigned long l = sizeLL(liste);
    if (index == l) {
        addLastLL(liste, element);
        return;
    }
    if (index > l) {
        printf("list out of bounds : %lu (taille : %lu)", index, l);
        exit(1);
    }
    liste->longueur++;
    LinkedList* position = liste->suiv;
    for (unsigned long i = 1; i < index; i++) {
        position = position->suiv;
    }
    LinkedList* nouv = newLinkedList();
    nouv->valeur = element;
    nouv->prec = position->prec;
    nouv->suiv = position;
    (position->prec)->suiv = nouv;
    position->prec = nouv;
}

void addAllLL(LinkedList* recepteur, LinkedList* source) {
    if (source->longueur == 0) {
        return;
    }
    LinkedList* position = source;
    while (position != NULL) {
        addLastLL(recepteur, position->valeur);
        position = position->suiv;
    }
}

void addAllLL2(LinkedList* recepteur, LinkedList* source) {
    printf("taille de la recepteur: %lu et du source:%lu\n",recepteur->longueur,source->longueur);
    afficheLinkedList(recepteur, 1);
    afficheLinkedList(source, 1);
    if (source->longueur == 0) {
        afficheLinkedList(recepteur, 1);
        return;
    }
    if (recepteur->longueur == 0) {
        recepteur->dernier=source->dernier;
        recepteur->suiv=source->suiv;
        recepteur->longueur=source->longueur;
        recepteur->valeur = source->valeur;
        afficheLinkedList(recepteur, 1);
        return;
        
    }
    LinkedList* nouv = newLinkedList();
    nouv->valeur = source->valeur;
    nouv->suiv = source->suiv;
    nouv->prec = source->prec;
    nouv->longueur = source->longueur;
    nouv->dernier = source->dernier;
    
    recepteur->dernier->suiv = nouv;
    nouv->prec = recepteur->dernier;
    recepteur->dernier = nouv->dernier;
    recepteur->longueur += nouv->longueur;
    afficheLinkedList(recepteur, 1);
    return;
}

void addLastCharLL(LinkedList* liste, char c) {
    char* element = malloc(sizeof (char));
    *element = c;
    addLastLL(liste, element);
}

void* getLastLL(LinkedList* liste) {
    return (liste->dernier)->valeur;
}

void* getFirstLL(LinkedList* liste) {
    return liste->valeur;
}

void* getLL(LinkedList* liste, unsigned long index) {
    if (index >= liste->longueur) {
        printf("list out of bounds : %lu (taille : %lu)", index, liste->longueur);
        exit(1);
    }
    LinkedList* position = liste;
    for (unsigned long i = 0; i < index; i++) {
        position = position->suiv;
    }
    return position->valeur;
}

char getCharLL(LinkedList* liste, unsigned long index) {
    return *((char*) getLL(liste, index));
}

void* removeLastLL(LinkedList* liste) {
    unsigned long l = liste->longueur;
    if (l == 0) {
        printf("La liste est deja vide");
        exit(2);
    }
    if (l == 1) {
        liste->longueur--;
        void* res = liste->valeur;
        liste->valeur = NULL;
        return res;
    }
    LinkedList* dernier = liste->dernier;
    (dernier->prec)->suiv = NULL;
    liste->dernier = dernier->prec;
    liste->longueur--;
    void* res = dernier->valeur;
    free(dernier);
    dernier = NULL;
    return res;
}

void* removeFirstLL(LinkedList* liste) {
    if (liste->longueur == 0) {
        printf("La liste est deja vide\n");
        exit(2);
    }
    if (liste->longueur == 1) {
        liste->longueur--;
        void* res = liste->valeur;
        liste->valeur = NULL;
        return res;
    }
    LinkedList* deuxieme = liste->suiv;
    void* res = liste->valeur;
    liste->valeur = deuxieme->valeur;
    liste->suiv = deuxieme->suiv;
    liste->longueur--;
    if (liste->dernier == deuxieme) {
        liste->dernier = liste;
    } else {
        (deuxieme->suiv)->prec = liste;
    }
    free(deuxieme);
    deuxieme = NULL;
    return res;
}

void* removeAtIndexLL(LinkedList* liste, unsigned long index) {
    if (index == 0) {
        return removeFirstLL(liste);
    }
    if (index == liste->longueur - 1) {
        return removeLastLL(liste);
    }
    liste->longueur--;
    LinkedList* aSupprimer = liste;
    for (unsigned long i = 0; i < index; i++) {
        aSupprimer = aSupprimer->suiv;
    }
    //ArrayList* aSupprimer = get(liste, index);
    (aSupprimer->prec)->suiv = aSupprimer->suiv;
    (aSupprimer->suiv)->prec = aSupprimer->prec;
    void* res = aSupprimer->valeur;
    free(aSupprimer);
    aSupprimer = NULL;
    return res;
}

void removeElementLL(LinkedList* liste, void* element) {
    unsigned long l = liste->longueur;
    LinkedList* position = liste;
    void* val = position->valeur;
    unsigned long i = 0;
    while (i < l && val != element) {
        i++;
        position = position->suiv;
        val = position->valeur;
    }
    if (val != element) { // en fait val = NULL
        printf("Element non present dans la liste");
        exit(3);
    }
    if (i == 0) {
        removeFirstLL(liste);
        return;
    }
    if (i == l - 1) {
        removeLastLL(liste);
        return;
    }
    liste->longueur--;
    (position->prec)->suiv = position->suiv;
    (position->suiv)->prec = position->prec;
    free(position);
    position = NULL;
}

unsigned long indexOfLL(LinkedList* liste, void* element) {
    unsigned long l = liste->longueur;
    unsigned long index = 0;
    LinkedList* position = liste;
    void* val = position->valeur;
    while (index < l && val != element) {
        index++;
        position = position->suiv;
        val = position->valeur;
    }
    if (val != element) { // en fait val = NULL
        printf("Element non present dans la liste");
        exit(3);
    }
    return index;
}

/**
 *
 * @param liste
 * @param element
 * @return
 */
long indexOfCharLL(LinkedList* liste, char element) {
    unsigned long l = liste->longueur;
    if (l == 0) {
        return -1;
    }
    long index = 0;
    LinkedList* position = liste;
    char val = *((char*) position->valeur);
    while (index < l && val != element) {
        index++;
        position = position->suiv;
        if (position != NULL) {
            val = *((char*) position->valeur);
        }
    }
    if (val != element) { // en fait val = NULL
        return -1;
    }
    return index;
}

void terminateLinkedList(LinkedList* liste) {
    LinkedList* position = liste->dernier;
    while (position != liste) {
        LinkedList* prec = position->prec;
        free(position);
        position = prec;
    }
    free(liste);
    liste = NULL;
}

void terminateLinkedListTotal(LinkedList* liste) {
    LinkedList* position = liste->dernier;
    while (position != liste) {
        LinkedList* prec = position->prec;
        free(position->valeur);
        free(position);
        position = prec;
    }
    free(liste);
    liste = NULL;
}

void afficheLinkedList(LinkedList* liste, short tailleElement) {
    if (liste->longueur == 0) {
        printf("[]\n");
        return;
    }
    printf("[");
    if (tailleElement == sizeof (char)) {
        LinkedList* position = liste;
        while (position != NULL) {
            char val = *((char*) position->valeur);
            printf("%d", val);
            if (position->suiv != NULL) {
                printf(", ");
            }
            position = position->suiv;
        }
    } else if (tailleElement == sizeof (int)) {
        LinkedList* position = liste;
        while (position != NULL) {
            int val = *((int*) position->valeur);
            printf("%d", val);
            if (position->suiv != NULL) {
                printf(", ");
            }
            position = position->suiv;
        }
    } else if (tailleElement == sizeof (long)) {
        LinkedList* position = liste;
        while (position != NULL) {
            long val = *((long*) position->valeur);
            printf("%ld", val);
            if (position->suiv != NULL) {
                printf(", ");
            }
            position = position->suiv;
        }
    }
    printf("]\n");
}

unsigned long compare(LinkedList* l1, LinkedList* l2){
    if (sizeLL(l1) != sizeLL(l2)) {
        return -1;
    } 
    LinkedList* t1 = l1;
    LinkedList* t2 = l2;
    for (int i = 0; i<sizeLL(l1); i++) {
        if (t1->valeur != t2->valeur) {
            return i+1;
        }
        t1 = t1->suiv;
        t2 = t2->suiv;
    }
    return 0; // les deux listes sont égales
}

//----------- EXPERIMENTAL-------------------------------

LinkedList* copieLinkedList(LinkedList* liste) {
    if (liste == NULL) {
        return NULL;
    }
    LinkedList* nouv = newLinkedList();
    nouv->valeur = liste->valeur;
    nouv->suiv = copieLinkedList(liste->suiv);
    if (nouv->suiv != NULL) {
        (nouv->suiv)->prec = nouv;
        nouv->dernier = (nouv->suiv)->dernier;
    } else {
        nouv->dernier = nouv;
    }
    return nouv;
}
