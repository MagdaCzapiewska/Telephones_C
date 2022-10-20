/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
 * @date 2022
 */

 #include <stdio.h>
 #include <stdbool.h>
 #include <stddef.h>
 #include <ctype.h>
 #include <stdlib.h>
 #include <string.h>
 #include "phone_forward.h"
 #include "phfwd_auxiliary_functions.h"
 #include "list.h"


 /** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward * phfwdNew(void) {
    PhoneForward *result = malloc(sizeof(*result));
    if (result != NULL) {
        Node *n = newNode(NULL);
        if (n != NULL) {
            result->forward = n;
            Node *m = newNode(NULL);
            if (m != NULL) {
                result->reverse = m;
            }
            else {
                free(n->sons);
                free(n);
                free(result);
                result = NULL;
            }
        }
        else {
            free(result);
            result = NULL;
        }
    }
    return result;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf - wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf) {
    if (pf != NULL) {
        treeDelete(pf->forward);
        treeDelete(pf->reverse);
        free(pf);
    }
}

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
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if((pf != NULL) && onlyDigitsAndNotEmpty(num1) && onlyDigitsAndNotEmpty(num2) && numbersDiffer(num1, num2)) {
        Node *first_added = NULL;
        Node *help = NULL;
        if (lookForANode(pf->forward, num1, &first_added, &help)) {
            if (changeForward(help, num2)) {
                Node *first_added_reverse = NULL;
                Node *help_reverse = NULL;
                if (lookForANode(pf->reverse, num2, &first_added_reverse, &help_reverse)) {
                    if (help_reverse->list == NULL) {
                        help_reverse->list = newList();
                    }
                    if (help_reverse->list == NULL) {
                        treeDelete(first_added_reverse);
                        treeDelete(first_added);
                        return false;
                    }
                    else {
                        if (addElement(help_reverse->list, num1, howLong(num1))) {
                            help->infoAboutMe = help_reverse;
                            help->imHere = help_reverse->list->last;
                            return true;
                        }
                        else {
                            if (empty(help_reverse->list)) {
                                free(help_reverse->list);
                                help_reverse->list = NULL;
                            }
                            treeDelete(first_added_reverse);
                            treeDelete(first_added);
                            return false;
                        }
                    }
                }
                else {
                    treeDelete(first_added);
                    return false;
                }
            }
            else {
                treeDelete(first_added);
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań,
 * napis nie reprezentuje numeru lub parametr pf ma wartość NULL, nic nie robi.
 * @param[in,out] pf - wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    - wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num) {
    if ((pf == NULL) || (num == NULL) || (*num == '\0')) {
        return;
    }
    char const *digit = num;
    Node *help = pf->forward;
    Node *father = NULL;
    size_t i = 0;
    if (onlyDigitsAndNotEmpty(num)) {
        size_t enough = howLong(num);
        while ((help != NULL) && (i < enough)) {
            help = (help->sons)[digitValue(digit)];
            ++digit;
            ++i;
        }
        if (i == enough) {
            if (help != NULL) {
                father = help->parent;
            }
            treeDelete(help);
            if (help != NULL) {
                removeEmptyBranch(father, father->parent);
            }
        }

    }
}

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
PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL) {
        return NULL;
    }

    PhoneNumbers *result = malloc(sizeof(*result));
    if (result != NULL) {
        ListOfNumbers *result_list = newList();
        if (result_list != NULL) {
            char *result_number = NULL;
            if (onlyDigitsAndNotEmpty(num)) {
                Node *last_modification = NULL;
                size_t how_many_digits_eaten = 0;
                lookForModification(pf->forward, &last_modification, &how_many_digits_eaten, num);
                if (last_modification == NULL) {
                    result_number = malloc((howLong(num) + 1) * sizeof(char));
                    if (result_number == NULL) {
                        free(result_list);
                        free(result);
                        result = NULL;
                        return result;
                    }
                    else {
                        size_t j = howLong(num);
                        for (size_t i = 0; i < j; ++i) {
                            result_number[i] = num[i];
                        }
                        result_number[j] = '\0';
                    }
                    if (!addElement(result_list, result_number, howLong(num) + 1)) {
                        free(result_number);
                        free(result_list);
                        free(result);
                        result = NULL;
                        return result;
                    }
                    else {
                        free(result_number);
                        result->list = result_list;
                    }
                }
                else {
                    size_t forward_length = howLong(num) - how_many_digits_eaten + last_modification->list->first->number_length;
                    result_number = malloc((forward_length + 1) * sizeof(char));
                    if (result_number == NULL) {
                        free(result_list);
                        free(result);
                        result = NULL;
                        return result;
                    }
                    else {
                        size_t j = howLong(num);
                        for (size_t i = 0; i < last_modification->list->first->number_length; ++i) {
                            result_number[i] = (last_modification->list->first->number)[i];
                        }
                        size_t k = last_modification->list->first->number_length - how_many_digits_eaten;
                        for (size_t i = how_many_digits_eaten; i < j; ++i) {
                            result_number[i + k] = num[i];
                        }
                        result_number[j + k] = '\0';
                    }
                    if (!addElement(result_list, result_number, forward_length + 1)) {
                        free(result_number);
                        free(result_list);
                        free(result);
                        result = NULL;
                        return result;
                    }
                    else {
                        free(result_number);
                        result->list = result_list;
                    }
                }
            }
            else {
                if (addElement(result_list, result_number, 0)) {
                    result->list = result_list;
                }
                else {
                    free(result_list);
                    free(result);
                    result = NULL;
                    return result;
                }
            }
        }
        else {
            free(result);
            result = NULL;
        }
    }
    return result;
}

/** @brief Tworzy tablicę numerów, które są zawarte w wyniku funkcji phfwdReverse lub phfwdGetReverse.
 * Tworzy tablicę numerów, które są zawarte w wyniku funkcji phfwdReverse lub phfwdGetReverse.
 * Numery te mogą się powtarzać i nie są posortowane leksykograficznie.
 * @param[in] pf - wskaźnik na strukturę przechowującą przekierowania numerów telefonów
 * @param[in] num - wskaźnik na napis reprezentujący numer.
 * @param[in, out] how_many_elements - ilosć elementów w tablicy numerów
 * @param[in] only_counterimage - zmienna informująca czy wyznaczamy tylko przeciwobraz phfwdGet
 * @return Wskaźnik na tablicę numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
char ** createArrayOfResults(PhoneForward const *pf, char const *num, size_t *how_many_elements, bool only_counterimage) {
    char **array = malloc(sizeof(*array));
    size_t how_long = howLong(num);
    if (array == NULL) {
        return array;
    }
    size_t array_size = 1;
    size_t number_of_elements = 0;

    Node *help = pf->reverse;
    char const *digit = num;
    char *help_number = malloc((how_long + 1) * sizeof(*help_number));

    if (help_number == NULL) {
        for (size_t j = 0; j < number_of_elements; ++j) {
            free(array[j]);
        }
        free(array);
        array = NULL;
        return array;
    }
    else {
        PhoneNumbers *pnum = NULL;
        pnum = phfwdGet(pf, num);
        if (pnum == NULL) {
            free(help_number);
            for (size_t j = 0; j < number_of_elements; ++j) {
                free(array[j]);
            }
            free(array);
            array = NULL;
            return array;
        }
        else {
            if (!only_counterimage || (only_counterimage && (strcmp(phnumGet(pnum, 0), num) == 0))) {
                for (size_t j = 0; j < how_long; ++j) {
                    help_number[j] = num[j];
                }
                help_number[how_long] = '\0';

                array[number_of_elements] = help_number;
                ++number_of_elements;
            }
            else {
                free(help_number);
            }
            phnumDelete(pnum);
        }
    }
    size_t i = 1;
    char **new_array = NULL;
    while ((help != NULL) && (*digit != '\0')) {
        help = (help->sons)[digitValue(digit)];
           
        OneNumber *element = NULL;
        if ((help != NULL) && (help->list != NULL)) {
            element = help->list->first;
        }
        while (element != NULL) {
            char *helping_number = malloc((how_long - i + element->number_length + 1) * sizeof(*helping_number));
            if (helping_number == NULL) {
                for (size_t j = 0; j < number_of_elements; ++j) {
                    free(array[j]);
                }
                free(array);
                array = NULL;
                return array;
            }
            else {
                for (size_t j = 0; j < element->number_length; ++j) {
                    helping_number[j] = (element->number)[j];
                }
                size_t index = element->number_length;
                for (size_t j = i; j < how_long; ++j) {
                    helping_number[index] = num[j];
                    ++index;
                }
                helping_number[element->number_length + how_long - i] = '\0';

                PhoneNumbers *pnum = NULL;
                pnum = phfwdGet(pf, helping_number);
                if (pnum == NULL) {
                    free(helping_number);
                    for (size_t j = 0; j < number_of_elements; ++j) {
                        free(array[j]);
                    }
                    free(array);
                    array = NULL;
                    return array;
                }
                else {
                    if (!only_counterimage || (only_counterimage && (strcmp(phnumGet(pnum, 0), num) == 0))) {
                        if (number_of_elements == array_size) {
                            new_array = realloc(array, more(array_size) * sizeof(*array));
                            array_size = more(array_size);

                            if (new_array == NULL) {
                                free(helping_number);
                                for (size_t j = 0; j < number_of_elements; ++j) {
                                    free(array[j]);
                                }
                                free(array);
                                array = NULL;
                                phnumDelete(pnum);
                                return array;
                            }
                            else {
                                array = new_array;
                                array[number_of_elements] = helping_number;
                                ++number_of_elements;
                            }
                        }
                        else {
                            array[number_of_elements] = helping_number;
                            ++number_of_elements;
                        }
                    }
                    else {
                        free(helping_number);
                    }
                    phnumDelete(pnum);
                }
            }
            element = element->next;
        }
        ++digit;
        ++i;
    }
    
    char **result = realloc(array, number_of_elements * sizeof(*array));
    if (result == NULL) {
        for (size_t i = 0; i < number_of_elements; ++i) {
            free(array[i]);
        }
        free(array);
        array = NULL;
    }
    else {
        *how_many_elements = number_of_elements;
        array = result;
    }

    return array;
}

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
PhoneNumbers * phfwdReverseOrGetReverse(PhoneForward const *pf, char const *num, bool only_counterimage) {
    if (pf == NULL) {
        return NULL;
    }

    PhoneNumbers *result = malloc(sizeof(*result));
    if (result != NULL) {
        ListOfNumbers *result_list = newList();
        if (result_list != NULL) {
            if (onlyDigitsAndNotEmpty(num)) {
                size_t how_many_elements = 0;
                char **array = createArrayOfResults(pf, num, &how_many_elements, only_counterimage);
                if (array != NULL) {
                    qsort(array, how_many_elements, sizeof(*array), myCompare);
                    if (how_many_elements > 0) {
                        if (addElement(result_list, array[0], howLong(array[0]))) {
                            bool go_on = true;
                            size_t i = 1;
                            while (go_on && (i < how_many_elements)) {
                                if (numbersDiffer(array[i], array[i-1])) {
                                    go_on = addElement(result_list, array[i], howLong(array[i]));
                                }
                                ++i;
                            }

                            for (size_t i = 0; i < how_many_elements; ++i) {
                                free(array[i]);
                            }
                            free(array);

                            if (!go_on) {
                                freeList(result_list);
                                free(result_list);
                                free(result);
                                result = NULL;
                            }
                            else {
                                result->list = result_list;
                            }
                        }
                        else {
                            for (size_t i = 0; i < how_many_elements; ++i) {
                                free(array[i]);
                            }
                            free(array);
                            free(result_list);
                            free(result);
                            result = NULL;
                        }
                    }
                    else {
                        if (addElement(result_list, NULL, 0)) {
                            result->list = result_list;
                        }
                        else {
                            free(result_list);
                            free(result);
                            result = NULL;
                            return result;
                        }
                    }
                }
                else {
                    free(result_list);
                    free(result);
                    result = NULL;
                }
            }
            else {
                if (addElement(result_list, NULL, 0)) {
                    result->list = result_list;
                }
                else {
                    free(result_list);
                    free(result);
                    result = NULL;
                    return result;
                }
            }
        }
        else {
            free(result);
            result = NULL;
        }
    }
    return result;
}

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
PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
    return phfwdReverseOrGetReverse(pf, num, 0);
}

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
PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
    return phfwdReverseOrGetReverse(pf, num, 1);
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum - wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers *pnum) {
    if (pnum != NULL) {
        if (pnum->list != NULL) {
            freeList(pnum->list);
        }
        free(pnum->list);
        pnum->list = NULL;
        free(pnum);
    }
}

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum - wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  - indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const * phnumGet(PhoneNumbers const *pnum, size_t idx) {
    char const *result = NULL;
    if ((pnum != NULL) && (idx < pnum->list->list_size)) {
        OneNumber *help = pnum->list->first;
        size_t i = 0;
        while (i < idx) {
            help = help->next;
            ++i;
        }
        result = help->number;
    }
    return result;
}
