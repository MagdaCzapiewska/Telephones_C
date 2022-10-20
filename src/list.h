/** @file
 * Interfejs klasy funkcji wykonujących operacje na liście.
 *
 * @author Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @date 2022
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * To jest struktura reprezentująca węzeł listy numerów.
 */
typedef struct OneNumber {
    char *number; ///< wskaźnik na początek tablicy, gdzie zapisany jest numer
    size_t number_length; ///< długość zapisanego w tym węźle listy numeru
    struct OneNumber *prev; ///< prev - wskaźnik na poprzedni węzeł listy
    struct OneNumber *next; ///< next - wskaźnik na kolejny węzeł listy
} OneNumber;

/**
 * To jest struktura reprezentująca listę numerów.
 */
typedef struct ListOfNumbers {
    struct OneNumber *first; ///< first - wskaźnik na pierwszy węzeł listy
    struct OneNumber *last; ///< last - wskaźnik na ostatni węzeł listy
    size_t list_size; ///< ilość elementów listy
} ListOfNumbers;

/** @brief Tworzy nowy węzeł listy numerów.
 * Tworzy nowy węzeł listy numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
OneNumber * newNumber();

/** @brief Tworzy nową listę numerów.
 * Tworzy nową listę numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
ListOfNumbers * newList();

/** @brief Sprawdza, czy lista jest pusta.
 * Sprawdza, czy lista jest pusta
 * @return Wartość @p true, gdy lista jest pusta; @p false w przeciwnym przypadku.
 */
bool empty(ListOfNumbers *list);

/** @brief Dodaje nowy element na koniec listy.
 * Dodaje nowy element na koniec listy.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę
 * @param[in] num - wskaźnik na ciąg znaków reprezentujący numer
 * @return Wartość @p true, gdy udało się alokować pamięć; @p false w przeciwnym przypadku.
 */
bool addElement(ListOfNumbers *list, const char *num, size_t number_length);

/** @brief Usuwa z listy element o podanym adresie.
 * Usuwa z listy element o podanym adresie.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę;
 * @param[in] element- wskaźnik na usuwany element
 */
void removeElement(ListOfNumbers *list, OneNumber *element);

/** @brief Usuwa listę.
 * Usuwa listę.
 * @param[in] list - wskaźnik na strukturę reprezentującą listę
 */
void freeList(ListOfNumbers *list);

#endif /* __LIST_H__ */