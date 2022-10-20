/** @file
 * Implementacja klasy funkcji wykonujących operacje na liście.
 *
 * @author Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @date 2022
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "list.h"

/** @brief Tworzy nowy węzeł listy numerów.
 * Tworzy nowy węzeł listy numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
OneNumber * newNumber() {
    OneNumber *result = malloc(sizeof(*result));
    if (result != NULL) {
        result->number = NULL;
        result->number_length = 0;
        result->prev = NULL;
        result->next = NULL;
    }
    return result;
}

/** @brief Tworzy nową listę numerów.
 * Tworzy nową listę numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
ListOfNumbers * newList() {
    ListOfNumbers *result = malloc(sizeof(*result));
    if (result != NULL) {
        result->first = NULL;
        result->last = NULL;
        result->list_size = 0;
    }
    return result;
}

/** @brief Sprawdza, czy lista jest pusta.
 * Sprawdza, czy lista jest pusta
 * @return Wartość @p true, gdy lista jest pusta; @p false w przeciwnym przypadku.
 */
bool empty(ListOfNumbers *list) {
    if (list->list_size == 0) {
        return true;
    }
    else {
        return false;
    }
}

/** @brief Dodaje nowy element na koniec listy.
 * Dodaje nowy element na koniec listy.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę
 * @param[in] num - wskaźnik na ciąg znaków reprezentujący numer
 * @return Wartość @p true, gdy udało się alokować pamięć; @p false w przeciwnym przypadku.
 */
bool addElement(ListOfNumbers *list, const char *num, size_t number_length) {
    OneNumber *help = malloc(sizeof(*help));
    if (help != NULL) {
        char *num_help = NULL;
        if (num != NULL) {
            num_help = malloc((number_length + 1) * sizeof(*num_help));
            if (num_help == NULL) {
                free(help);
                return false;
            }
            for (size_t i = 0; i < number_length; ++i) {
                num_help[i] = num[i];
            }
            num_help[number_length] = '\0';
        }
        help->number = num_help;
        help->number_length = number_length;
        help->next = NULL;

        if(empty(list)) {
            help->prev = NULL;
            list->first = help;
            list->last = help;
        }
        else {
            help->prev = list->last;
            (list->last)->next = help;
            list->last = help;
        }
        ++(list->list_size);
        return true;
    }
    else {
        return false;
    }
}

/** @brief Usuwa z listy element o podanym adresie.
 * Usuwa z listy element o podanym adresie.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę;
 * @param[in] element- wskaźnik na usuwany element
 */
void removeElement(ListOfNumbers *list, OneNumber *element) {
    if ((list != NULL) && (element != NULL)) {
        if(!empty(list)) {
            free(element->number);
            if ((list->first != element) && (list->last != element)) { // Element jest w środku listy.
                (element->prev)->next = element->next;
                (element->next)->prev = element->prev;
            }
            if (list->first == element) {
                list->first = element->next; // W przypadku listy jednoelementowej to będzie NULL.
                if (list->first != NULL) {
                    (list->first)->prev = NULL;
                }
            }
            if (list->last == element) {
                list->last = element->prev; // W przypadku listy jednoelementowej to będzie NULL.
                if (list->last != NULL) {
                    (list->last)->next = NULL;
                }
            }
            free(element);
            --(list->list_size);
        }
    }

}

/** @brief Usuwa listę.
 * Usuwa listę.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę
 * Nic nie robi, jeśli wskaźnik jest równy NULL.
 */
void freeList(ListOfNumbers *list) {
    if (list != NULL) {
        while(!empty(list)) {
            removeElement(list, list->last);
        }
    }
}