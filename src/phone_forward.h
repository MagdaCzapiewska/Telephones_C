/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @authors Marcin Peczarski <marpe@mimuw.edu.pl>, Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "phfwd_auxiliary_functions.h"
#include "list.h"

/**
 * To jest stała o wartości równej ilości cyfr, z których składa się alfabet cyfr, nad którym są numery
 */
#define SONS 12

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
typedef struct PhoneForward {
    struct Node *forward; ///< wskaźnik na węzeł będący korzeniem drzewa przekierowań
    struct Node *reverse; ///< wskaźnik na węzeł będący korzeniem drzewa odwróceń
} PhoneForward;

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
typedef struct PhoneNumbers {
    struct ListOfNumbers *list; ///< lista numerów
} PhoneNumbers;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward * phfwdNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf - wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf - wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num1   - wskaźnik na napis reprezentujący prefiks numerów
 *                     przekierowywanych;
 * @param[in] num2   - wskaźnik na napis reprezentujący prefiks numerów,
 *                     na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. parametr pf ma wartość NULL,
 *         podany napis nie reprezentuje numeru, oba podane numery są identyczne
 *         lub nie udało się alokować pamięci.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań,
 * napis nie reprezentuje numeru lub parametr pf ma wartość NULL, nic nie robi.
 * @param[in,out] pf - wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    - wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  - wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num - wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo parametr pf ma wartość NULL.
 */
PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Tworzy tablicę numerów, które są zawarte w wyniku funkcji phfwdReverse lub phfwdGetReverse.
 * Tworzy tablicę numerów, które są zawarte w wyniku funkcji phfwdReverse lub phfwdGetReverse.
 * Numery te mogą się powtarzać i nie są posortowane leksykograficznie.
 * @param[in] pf - wskaźnik na strukturę przechowującą przekierowania numerów telefonów
 * @param[in] num - wskaźnik na napis reprezentujący numer
 * @param[in, out] how_many_elements - ilosć elementów w tablicy numerów
 * @param[in] only_counterimage - zmienna informująca czy wyznaczamy tylko przeciwobraz phfwdGet
 * @return Wskaźnik na tablicę numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
char ** createArrayOfResults(PhoneForward const *pf, char const *num, size_t *how_many_elements, bool only_counterimage);

/** @brief Wyznacza wynik funkcji phfwdReverse lub phfwdGetReverse.
 * Wyznacza wynik funkcji phfwdReverse lub phfwdGetReverse w zależności
 * od wartości @p only_counterimage. Jeśli wartość ta wynosi true, wyznacza
 * wynik funkcji phfwdGetReverse. Jeśli false, wyznacza wynik funkcji phfwdReverse. 
 * @param[in] pf  - wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num - wskaźnik na napis reprezentujący numer;
 * @param[in] only_counterimage - zmienna informująca o tym, czy wyznaczamy tylko przeciwobraz phfwdGet
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo parametr pf ma wartość NULL.
 */
PhoneNumbers * phfwdReverseOrGetReverse(PhoneForward const *pf, char const *num, bool only_counterimage);

/** @brief Wyznacza ciąg takich numerów, które są przekierowywane na prefiks danego numeru, zmodyfikowanych przez dodanie odpowiedniego sufiksu.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że wynik
 * wywołania @p phfwdGet z numerem @p x zawiera numer będący prefiksem @p num,
 * to do wyniku wywołania @ref phfwdReverse z numerem @p num należy numer będący
 * konkatenacją numeru @p x i numeru, który powstaje w wyniku odjęcia od @p num prefiksu
 * równego wynikowi wywołania @p phfwdGet z numerem @p x.
 * Dodatkowo ciąg wynikowy zawsze zawiera też numer @p num. Wynikowe numery są posortowane
 * leksykograficznie i nie mogą się powtarzać. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  - wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num - wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo parametr pf ma wartość NULL.
 */
PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że wynik
 * wywołania @p phfwdGet z numerem @p x zawiera numer @p num, to numer @p x
 * należy do wyniku wywołania @ref phfwdReverse z numerem @p num.
 * Wynikowe numery są posortowane leksykograficznie i nie mogą się powtarzać.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  - wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num - wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo parametr pf ma wartość NULL.
 */
PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum - wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum - wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  - indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const * phnumGet(PhoneNumbers const *pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */