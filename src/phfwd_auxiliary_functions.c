/** @file
 * Implementacja klasy funkcji pomocniczych klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @date 2022
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "phfwd_auxiliary_functions.h"
#include "phone_forward.h"
#include "list.h"

/** @brief Tworzy nowy węzeł drzewa przekierowań.
 * Tworzy nowy węzeł drzewa przekierowań.
 * @param[in] father - wskaźnik na węzeł, który jest ojcem tworzonego węzła
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
Node * newNode(Node *father) {
    Node *result = malloc(sizeof(*result));
    if (result != NULL) {
        result->parent = father;
        result->list = NULL;
        result->infoAboutMe = NULL;
        result->imHere = NULL;

        result->sons = malloc(SONS * sizeof(*(result->sons)));
        if (result->sons != NULL) {
            for (int i = 0; i < SONS; ++i) {
                (result->sons)[i] = NULL;
            }
        }
        else {
            free(result);
            result = NULL;
        }
    }
    
    return result;
}

/** @brief Usuwa drzewo przekierowań.
 * Usuwa drzewo przekierowań, którego korzeń wskazywany jest przez @p n.
 * W szczególności może to być poddrzewo innego drzewa.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] n - wskaźnik na korzeń usuwanego drzewa.
 */
void treeDelete(Node *n) {
    if (n != NULL) {
        Node *help1 = n;
        Node *help2 = NULL;
        Node *not_to_delete = (help1->parent);
        int index;
        while (help1 != not_to_delete) {
            while (!isLeaf(help1)) {
                help1 = leftChild(help1);
            }
            if (help1->parent != NULL) {
                index = whichChild(help1);
                help2 = (help1->parent);

                freeList(help1->list); // Jeżli help1->list == NULL, funkcja freeList() nic nie zrobi.
                free(help1->list);
                help1->list = NULL;

                if (help1->infoAboutMe != NULL) { // W węzłach drzewa reverse zawsze infoAboutMe == NULL.
                    removeElement(help1->infoAboutMe->list, help1->imHere);
                    if (empty(help1->infoAboutMe->list)) {
                        freeList(help1->infoAboutMe->list);
                        free(help1->infoAboutMe->list);
                        help1->infoAboutMe->list = NULL;
                        removeEmptyBranch(help1->infoAboutMe, help1->infoAboutMe->parent);
                    }
                }

                free(help1->sons);
                free(help1);
                (help2->sons)[index] = NULL;
                help1 = help2;
            }
            else {
                help2 = (help1->parent);
                free(help1->sons);
                free(help1);
                help1 = help2;
            }
        }
    }
}

/** @brief Usuwa martwą gałąź drzewa.
 * Usuwa martwą gałąź drzewa.
 * @param[in] help - wskaźnik na liść gałęzi
 * @param[in] father - wskaźnik na ojca liścia gałęzi
 */
void removeEmptyBranch(Node *help, Node *father) {
    if (help == NULL) {
        return;
    }
    bool keep_going = true;
    while (keep_going && (father != NULL)) {
            if (isLeaf(help) && (help->list == NULL)) {
                treeDelete(help);
                father = father->parent;
                help = father;
            }
            else {
                keep_going = false;
            }
    }
}

/** @brief Sprawdza, czy węzeł drzewa jest liściem.
 * @param[in] n - wskaźnik na węzeł drzewa
 * @return Wartość @p true, jeśli węzeł jest liściem.
 *         Wartość @p false, jeśli węzeł nie jest liściem.
 */
bool isLeaf(Node *n) {
    for (int i = 0; i < SONS; ++i) {
        if ((n->sons)[i] != NULL) {
            return false;
        }
    }
    return true;
}

/** @brief Sprawdza, jak długi jest napis reprezentujący numer.
 * @param[in] num - wskaźnik na napis reprezentujący numer
 * @return Liczba znaków będących cyframi, które zawiera dany numer.
 */
size_t howLong(char const *num) {
    char const *help = num;
    size_t i = 0;
    while (*help != '\0') {
        ++i;
        ++help;
    }
    return i;
}

/** @brief Wskazuje adres dziecka węzła danego jako argument, którego adres jest różny od NULL.
 * Wskazuje adres dziecka węzła danego jako argument, którego adres jest różny od NULL.
 * W przypadku kilkorga takich dzieci, wybiera to, które znajduje się w komórce tablicy o najmniejszym indeksie.
 * @param[in] n - wskaźnik na węzeł drzewa
 * @return Wskaźnik na wybrane dziecko. Jeśli nie ma już dzieci, NULL. Jednak tą funkcję wywołujemy
 * po sprawdzeniu, że argument nie jest liściem, więc w praktyce nie zwróci nigdy wartości NULL.
 */
Node * leftChild(Node *n) {
    Node *result = NULL;
    int i = 0;
    while ((result == NULL) && (i < SONS)) {
        if ((n->sons)[i] != NULL) {
            result = (n->sons)[i];
        }
        ++i;
    }
    return result;
}

/** @brief Sprawdza, którym dzieckiem swojego rodzica jest dany węzeł.
 * @param[in] child - wskaźnik na węzeł drzewa, o którym chcemy wiedzieć, którym dzieckiem swojego rodzica jest
 * @return Indeks dziecka w tablicy dzieci rodzica
 */
int whichChild(Node *child) {
    Node *parent = child->parent;
    for (int i = 0; i < SONS; ++i) {
        if ((parent->sons)[i] == child) {
            return i;
        }
    }
    return -1;
}

/** @brief Sprawdza, czy napis reprezentuje numer.
 * @param[in] num - wskaźnik na napis potencjalnie reprezentujący numer
 * @return Wartość @p true, jeśli napis reprezentuje numer.
 *         Wartość @p false, jeśli napis nie reprezentuje numeru.
 */
bool onlyDigitsAndNotEmpty(char const *num) {
    if (num == NULL) {
        return false;
    }
    char const *help = num;
    if (*help == '\0') {
        return false;
    }
    while (isdigit((int)(*help)) || (*help == '*') || (*help == '#')) {
        ++help;
    }
    if (*help == '\0') {
        return true;
    }
    else {
        return false;
    }
}

/** @brief Zwraca wartość z zakresu od 0 do 11, reprezentowaną przez cyfrę.
 * @param[in] digit - wskaźnik na znak reprezentujacy cyfrę
 * @return Liczba całkowita z zakresu od 0 do 11, którą reprezentuje cyfra.
 */
int digitValue(char const *digit) {
    if (*digit == '*') {
        return 10;
    }
    else if (*digit == '#') {
        return 11;
    }
    else {
        return ((int)(*digit) - (int)('0'));
    }
}

/** @brief Sprawdza, czy napisy są różne.
 * @param[in] num1 - wskaźnik na napis reprezentujący pierwszy numer
 * @param[in] num2 - wskaźnik na napis reprezentujacy drugi numer
 * @return Wartość @p true, jeśli napisy są różne.
 *         Wartość @p false, jeśli napisy są równe.
 *
 */
bool numbersDiffer(char const *num1, char const *num2) {
    char const *help1 = num1;
    char const *help2 = num2;

    if ((*help1 == '\0') || (*help2 == '\0')) {
        return false;
    }

    while((*help1 == *help2) && (*help1 != '\0') && (*help2 != '\0')) {
        ++help1;
        ++help2;
    }
    if ((*help1 == '\0') && (*help2 == '\0')) {
        return false;
    }
    else {
        return true;
    }
}

/** @brief Szuka węzła drzewa wyznaczanego przez daną ścieżkę.
 * Szuka węzła drzewa wyznaczanego przez daną ścieżkę.
 * Jeśli takiego węzła nie ma, tworzy go.
 * @param[in,out] root - wskaźnik na węzeł drzewa
 * @param[in] num - wskaźnik na napis reprezentujący ścieżkę
 * @param[in, out] first_added - adres zmiennej, na której zostaje zapisany adres węzła, który został dodany jako pierwszy
 * @param[in, out] result - adres zmiennej, na której zostaje zapisany adres znalezionego węzła
 * @return Wartość @p true, jeśli udało się odnaleźć lub dodać węzeł.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
bool lookForANode(Node *root, char const *num, Node **first_added, Node **result) {
    Node *help = root;
    char const *digit = num;
    while ((help != NULL) && (*digit != '\0')) {
        if ((help->sons)[digitValue(digit)] == NULL) {
            (help->sons)[digitValue(digit)] = newNode(help);
            if (*first_added == NULL) {
                *first_added = (help->sons)[digitValue(digit)];
            }
        }
        help = (help->sons)[digitValue(digit)];
        ++digit;
    }
    if (help == NULL) {
        treeDelete(*first_added);
        return false;
    }
    else {
        *result = help;
        return true;
    }
}

/** @brief Zmienia (dodaje lub zastępuje) przekierowanie danego numeru (i wszystkich numerów, których on jest prefiksem).
 * @param[in] n - wskaźnik na węzeł drzewa, w którym będzie zapisywane przekierowanie
 * @param[in] num - wskaźnik na napis reprezentujący numer, na który jest tworzone przekierowanie
 * @return Wartość @p true, jeśli udało się alokować pamięć.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
bool changeForward(Node *n, char const *num) {
    size_t i = howLong(num);
    char *new_forward = malloc(i * sizeof(*num));
    if (new_forward != NULL) {
        if (n->list == NULL) {
            n->list = newList();
        }
        if (n->list != NULL) {
            for (size_t j = 0; j < i; ++j) {
                new_forward[j] = num[j];
            }
            if (!empty(n->list)) {
                removeElement(n->list, (n->list)->last);
                removeElement(n->infoAboutMe->list, n->imHere);
                if (empty(n->infoAboutMe->list)) {
                    freeList(n->infoAboutMe->list);
                    free(n->infoAboutMe->list);
                    n->infoAboutMe->list = NULL;
                    removeEmptyBranch(n->infoAboutMe, n->infoAboutMe->parent);
                }
            }
            if (addElement(n->list, new_forward, i)) {
                free(new_forward);
                return true;
            }
            else {
                free(new_forward);
                return false;
            }

        }
        else {
            free(new_forward);
            return false; // Nie udało się alokować pamięci.
        }
        
    }
    else {
        return false;
    }
    return true;
}

/** @brief Szuka najdłuższego prefiksu num, który ma przekierowanie inne niż na samego siebie.
 * @param[in] n - wskaźnik na korzeń drzewa przekierowań
 * @param[in, out] last_modification - wskaźnik na zmienną zawierającą adres węzła drzewa,
 * w którym znalezione zostało najbardziej aktualne przekierowanie
 * @param[in, out] how_many_digits_eaten - wskaźnik na zmienną zawierającą informację o tym,
 * jaka jest długość ścieżki od węzła o adresie n do węzła o adresie *last_modification
 * @param[in] num - wskaźnik na napis reprezentujący numer, którego przekierowanie jest odczytywane
 */
void lookForModification(Node *n, Node **last_modification, size_t *how_many_digits_eaten, char const *num) {
    if (n != NULL) {
        char const *digit = num;
        Node *help = n;
        help = (help->sons)[digitValue(digit)];
        size_t how_many_steps_can_be_made = howLong(num);
        size_t how_many_steps_was_made = 1;

        while ((help != NULL) && (how_many_steps_was_made < how_many_steps_can_be_made)) {
            if (help->list != NULL) {
                *last_modification = help;
                *how_many_digits_eaten = how_many_steps_was_made;
            }
            ++digit;
            ++how_many_steps_was_made;
            help = (help->sons)[digitValue(digit)];
        }
        if (help != NULL) {
            if (help->list != NULL) {
                *last_modification = help;
                *how_many_digits_eaten = how_many_steps_was_made;
            }
        }
    }
}

/** @brief Zwraca liczbę około dwa razy większą od argumentu.
 * @param[in] argument - liczba całkowita
 * @return Liczba całkowita około dwa razy większa od argumentu
 */
size_t more(size_t argument) {
    return 2 * argument + 1;
}

/** @brief Porównuje 2 numery.
 * Porównuje 2 numery.
 * @param[in] num1  - wskaźnik na napis reprezentujący pierwszy numer
 * @param[in] num2 - wskaźnik na napis reprezentujący drugi numer
 * @return Wartość @p 0, gdy numery są równe, wartość @p 1, gdy pierwszy numer jest większy, a @p -1, gdy mniejszy.
 */
int myCompare(const void *num1, const void *num2) {
    char *number1 = *(char **)num1;
    char *number2 = *(char **)num2;
    while((*number1 != '\0') && (*number2 != '\0') && (*number1 == *number2)) {
        ++number1;
        ++number2;
    }
    if ((*number1 == '\0') && (*number2 == '\0')) {
        return 0;
    }
    else if (*number1 == '\0') {
        return -1;
    }
    else if (*number2 == '\0') {
        return 1;
    }
    else if (digitValue(number1) > digitValue(number2)) {
        return 1;
    }
    else {
        return -1;
    }
}