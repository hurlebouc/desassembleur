/** file LinkedList.h
 * 
 */


#ifndef desassembleur_LinkedList_h
#define desassembleur_LinkedList_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
    
    /**
     * Liste chainée
     */
    typedef struct LinkedList
    {
        void* valeur;/*!<Pointeur vers l'objet contenu par le maillon*/
        unsigned long longueur;/*!<taille de la liste totale*/
        struct LinkedList* prec;/*!<Prédécesseur dans la liste*/
        struct LinkedList* suiv;/*!<Succésseur dans la liste*/
        struct LinkedList* dernier;/*!<Dernier élément de la liste*/
    }LinkedList;
    
    /**
     * Instancie une nouvelle liste
     * @return la nouvelle instance
     */
    LinkedList* newLinkedList(void);
    
    /**
     * Crée une copie de la liste chainé. Il faut cependant faire attention comme
     * nous manipulon dans la liste le pointeur vers un objet, celui ci reste inchangé.
     * Par conséquent, si nous modifions les éléments depuis la liste originale,
     * les éléments de la liste copié avec cette méthode seront eux aussi modifiés.
     * @param[in] Liste à copier 
     * @return Copie de la liste
     */
    LinkedList* newLinkedListCopy(LinkedList*);
    
    /**
     * Donne la taille de la liste totale en nombre d'éléments.
     * @param[in] liste dont on veut la taille
     * @return taille de la liste.
     */
    unsigned long sizeLL(LinkedList* liste);
    
    /**
     * Position dans la liste en partant de la queue. Ou encore, taille de la liste
     * depuis le maillon jusqu'à la fin.
     * @param[in] liste Liste à mesure depuis CE maillon
     * @return Taille de la liste
     */
    unsigned long sizeExtLL(LinkedList* liste);
    
    /**
     * Ajoute un élément à la fin de la liste.
     * @param[in/out] liste Liste à laquel on veut rajouter l'élément.
     * @param element Elément à mettre en fin de liste
     */
    void addLastLL(LinkedList* liste, void* element);
    
    /**
     * 
     * @param[in/out] liste
     * @param[in] element
     */
    void addFirstLL(LinkedList* liste, void* element);
    
    /**
     * Insère un élément à un endroit donné de la liste.
     * @param[in/out] liste Liste cible
     * @param[in] element Elément à insérer
     * @param[in] index Index dans la liste où l'élement doit être inséré
     */
    void addLL(LinkedList* liste, void* element, unsigned long index);
    
    
    void addAllLL(LinkedList* recepteur, LinkedList* source);
    void addAllLL2(LinkedList* recepteur, LinkedList* source);
    /**
     * Ajout en dernière position de la liste d'un caractère
     * @param[in/out] liste Liste cible
     * @param[in] c Caractère à placer en queue de liste
     */
    void addLastCharLL(LinkedList* liste, char c);
    
    /**
     * Retourne le dernier élément d'une liste. Sans l'enlever.
     * @param[in] liste Liste cible
     * @return Dernier élément de la liste. Pointeur vers l'élément
     */
    void* getLastLL(LinkedList* liste);
    
    /**
     * Retourne la tête de la liste sans l'enlever.
     * @param[in] liste Liste cible
     * @return Tête de la liste
     */
    void* getFirstLL(LinkedList* liste);
    
    /**
     * Retourne un élément d'une liste sans l'enlever de la liste.
     * @param[in] liste Liste dont on récupère l'élément
     * @param[in] index Emplacement de l'élément dans la liste
     * @return Elément extrait
     */
    void* getLL(LinkedList* liste, unsigned long index);
    
    /**
     *Retourne un caractère d'une liste sans l'enlever de la liste.
     * @param[in] liste Liste dont on récupère l'élément
     * @param[in] index Emplacement de l'élément dans la liste
     * @return Caractère extrait
     */
    char getCharLL(LinkedList* liste, unsigned long index);
    
    /**
     * Renvoie le dernier élément de la liste et le retire.
     * @param[in] liste Liste cible
     * @return Elément récupéré
     */
    void* removeLastLL(LinkedList* liste);
    
    /**
     * Renvoie le premier élément de la liste et le retire
     * @param[in] liste Liste cible
     * @return Elément récupéré
     */
    void* removeFirstLL(LinkedList* liste);
    
    /**
     * Renvoie un élément d'une liste et le retire
     * @param[in] liste Liste cible
     * @param[in] index Emplacement de l'élément dans la liste
     * @return Elément récupéré
     */
    void* removeAtIndexLL(LinkedList* liste, unsigned long index);
    
    /**
     * Recherche et supprime un élément d'une liste
     * @param[in] liste Liste à parcourir
     * @param[in] element Elément à chercher et à supprimer si trouvé
     */
    void removeElementLL(LinkedList* liste, void* element);
    
    /**
     * Recherche l'index d'un élément d'une liste
     * @param[in] liste Liste à parcourir
     * @param[in] element Elément dont on cherche l'indice
     * @return Indice de l'élément si il a été trouvé
     */
    unsigned long indexOfLL(LinkedList* liste, void* element);
    
    /**
     * Recherche l'emplacement d'un caractère dans une liste chainée
     * @param[in] liste Liste à parcourir
     * @param[in] element Caractère dont on cherche l'indice
     * @return Indice du caractère si il a été trouvé
     */
    long indexOfCharLL(LinkedList* liste, char element);
    
    /**
     * Effacement de la liste, mais pas des éléments qu'elle contenait
     * @param[in] liste Liste à supprimer
     */
    void terminateLinkedList(LinkedList* liste);
    
    /**
     * Effacement de la liste et des éléments qu'elle contenait
     * @param[in] liste Liste à supprimer
     */
    void terminateLinkedListTotal(LinkedList* liste);
    
    /**
     * Affichage d'une liste
     * @param[in] liste Liste à afficher
     * @param[in] tailleElement Typage des éléments affiché
     */
    void afficheLinkedList(LinkedList* liste, short tailleElement);
    
    /**
     * Compare deux listes
     * @param l1
     * @param l2
     * @return Retourne 0 si les deux listes sont identiques
     */
    unsigned long compare(LinkedList* l1, LinkedList* l2);
    
    
    LinkedList* copieLinkedList(LinkedList* liste);
    /*
     a utiliser avec beaucoup de précautions car cette foncion
     ne copie pas les objets pointés par le pointeur valeur
     */
    
#ifdef __cplusplus
}
#endif

#endif

