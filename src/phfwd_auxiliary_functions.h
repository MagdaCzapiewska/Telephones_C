/** @file
 * Interfejs klasy funkcji pomocniczych klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @date 2022
 */

#ifndef __PHFWD_AUXILIARY_FUNCTIONS_H__
#define __PHFWD_AUXILIARY_FUNCTIONS_H__

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "phone_forward.h"
#include "list.h"

/**
 * To jest struktura przechowująca zawartość węzła drzewa przekierowań.
 */
typedef struct Node {
    struct Node **sons; ///< tablica wskaźników na węzły będące synami danego węzła
    struct Node *parent; ///< parent - wskaźnik na węzeł będący rodzicem danego węzła
    struct ListOfNumbers *list; ///< lista przekierowań lub odwróceń zapisywanych w danym węźle
    struct Node *infoAboutMe; ///< infoAboutMe - wskaźnik na węzeł w drzewie odwróceń z informacją o węźle przekierowań
    struct OneNumber *imHere; ///< imHere - wskaźnik na węzeł listy w drzewie odwróceń
} Node;

/** @brief Tworzy nowy węzeł drzewa przekierowań.
 * Tworzy nowy węzeł drzewa przekierowań.
 * @param[in] father - wskaźnik na węzeł, który jest ojcem tworzonego węzła
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
Node * newNode(Node *father);

/** @brief Usuwa drzewo przekierowań.
 * Usuwa drzewo przekierowań, którego korzeń wskazywany jest przez @p n.
 * W szczególności może to być poddrzewo innego drzewa.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] n - wskaźnik na korzeń usuwanego drzewa.
 */
void treeDelete(Node *n);

/** @brief Usuwa martwą gałąź drzewa.
 * Usuwa martwą gałąź drzewa.
 * @param[in] help - wskaźnik na liść gałęzi
 * @param[in] father - wskaźnik na ojca liścia gałęzi
 */
void removeEmptyBranch(Node *help, Node *father);

/** @brief Sprawdza, czy węzeł drzewa jest liściem.
 * @param[in] n - wskaźnik na węzeł drzewa
 * @return Wartość @p true, jeśli węzeł jest liściem.
 *         Wartość @p false, jeśli węzeł nie jest liściem.
 */
bool isLeaf(Node *n);

/** @brief Sprawdza, jak długi jest napis reprezentujący numer.
 * @param[in] num - wskaźnik na napis reprezentujący numer
 * @return Liczba znaków będących cyframi, które zawiera dany numer.
 */
size_t howLong(char const *num);

/** @brief Wskazuje adres dziecka węzła danego jako argument, którego adres jest różny od NULL.
 * Wskazuje adres dziecka węzła danego jako argument, którego adres jest różny od NULL.
 * W przypadku kilkorga takich dzieci, wybiera to, które znajduje się w komórce tablicy o najmniejszym indeksie.
 * @param[in] n - wskaźnik na węzeł drzewa
 * @return Wskaźnik na wybrane dziecko. Jeśli nie ma już dzieci, NULL. Jednak tą funkcję wywołujemy
 * po sprawdzeniu, że argument nie jest liściem, więc w praktyce nie zwróci nigdy wartości NULL.
 */
Node * leftChild(Node *n);

/** @brief Sprawdza, którym dzieckiem swojego rodzica jest dany węzeł.
 * @param[in] child - wskaźnik na węzeł drzewa, o którym chcemy wiedzieć, którym dzieckiem swojego rodzica jest
 * @return Indeks dziecka w tablicy dzieci rodzica
 */
int whichChild(Node *child);

/** @brief Sprawdza, czy napis reprezentuje numer.
 * @param[in] num - wskaźnik na napis potencjalnie reprezentujący numer
 * @return Wartość @p true, jeśli napis reprezentuje numer.
 *         Wartość @p false, jeśli napis nie reprezentuje numeru.
 */
bool onlyDigitsAndNotEmpty(char const *num);

/** @brief Zwraca wartość z zakresu od 0 do 11, reprezentowaną przez cyfrę.
 * @param[in] digit - wskaźnik na znak reprezentujacy cyfrę
 * @return Liczba całkowita z zakresu od 0 do 11, którą reprezentuje cyfra.
 */
int digitValue(char const *digit);

/** @brief Sprawdza, czy napisy są różne.
 * @param[in] num1 - wskaźnik na napis reprezentujący pierwszy numer
 * @param[in] num2 - wskaźnik na napis reprezentujacy drugi numer
 * @return Wartość @p true, jeśli napisy są różne.
 *         Wartość @p false, jeśli napisy są równe.
 *
 */
bool numbersDiffer(char const *num1, char const *num2);

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
bool lookForANode(Node *root, char const *num, Node **first_added, Node **result);

/** @brief Zmienia (dodaje lub zastępuje) przekierowanie danego numeru (i wszystkich numerów, których on jest prefiksem).
 * @param[in] n - wskaźnik na węzeł drzewa, w którym będzie zapisywane przekierowanie
 * @param[in] num - wskaźnik na napis reprezentujący numer, na który jest tworzone przekierowanie
 * @return Wartość @p true, jeśli udało się alokować pamięć.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
bool changeForward(Node *n, char const *num);

/** @brief Szuka najdłuższego prefiksu num, który ma przekierowanie inne niż na samego siebie.
 * @param[in] n - wskaźnik na korzeń drzewa przekierowań
 * @param[in, out] last_modification - wskaźnik na zmienną zawierającą adres węzła drzewa,
 * w którym znalezione zostało najbardziej aktualne przekierowanie
 * @param[in, out] how_many_digits_eaten - wskaźnik na zmienną zawierającą informację o tym,
 * jaka jest długość ścieżki od węzła o adresie n do węzła o adresie *last_modification
 * @param[in] num - wskaźnik na napis reprezentujący numer, którego przekierowanie jest odczytywane
 */
void lookForModification(Node *n, Node **last_modification, size_t *how_many_digits_eaten, char const *num);

/** @brief Zwraca liczbę około dwa razy większą od argumentu.
 * @param[in] argument - liczba całkowita
 * @return Liczba całkowita około dwa razy większa od argumentu
 */
size_t more(size_t argument);

/** @brief Porównuje 2 numery.
 * Porównuje 2 numery.
 * @param[in] num1  - wskaźnik na napis reprezentujący pierwszy numer
 * @param[in] num2 - wskaźnik na napis reprezentujący drugi numer
 * @return Wartość @p 0, gdy numery są równe, wartość @p 1, gdy pierwszy numer jest większy, a @p -1, gdy mniejszy.
 */
int myCompare(const void *num1, const void *num2);

#endif /* __PHFWD_AUXILIARY_FUNCTIONS_H__ */