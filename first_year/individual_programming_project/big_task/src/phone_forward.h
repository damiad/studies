/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Damian Dąbrowski <dd439954@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneForward;
typedef struct PhoneForward PhoneForward;  ///< Przekierowania jako PhoneForward

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers;
typedef struct PhoneNumbers PhoneNumbers; ///< Ciąg numerów jako PhoneNumbers

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward* phfwdNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward* pf);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num1   – wskaźnik na napis reprezentujący prefiks numerów
 *                     przekierowywanych;
 * @param[in] num2   – wskaźnik na napis reprezentujący prefiks numerów,
 *                     na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne, @p pf to NULL
 *         lub nie udało się alokować pamięci.
 */
bool phfwdAdd(PhoneForward* pf, char const* num1, char const* num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru lub @p pf to NULL, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward* pf, char const* num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci, bądź @p pf to NULL.
 */
PhoneNumbers* phfwdGet(PhoneForward const* pf, char const* num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że
 * przekierowuje on na numer będący prefiksem @p num, to numer @p x
 * należy do wyniku wywołania @ref phfwdReverse z numerem @p num. Dodatkowo ciąg
 * wynikowy zawsze zawiera też numer @p num. Wynikowe numery są posortowane
 * leksykograficznie i nie mogą się powtarzać. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci, bądź @p pf to NULL.
 */
PhoneNumbers* phfwdReverse(PhoneForward const* pf, char const* num);

/** @brief Przeciwobraz funkcji phfwdGet.
 * Funkcja dla podanej za pomocą wskaźnika pf bazy przekierowań i podanego numeru
 * telefonu num wyznacza posortowaną leksykograficznie listę wszystkich takich numerów
 * telefonów i tylko takich numerów telefonów x, że phfwdGet(x) = num. Funkcja ta alokuje
 * strukturę @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * Wynikiem funkcji jest NULL, gdy nie udało się alokować pamięci.
 * Jeśli podany napis num nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci, bądź @p pf to NULL.
 */
PhoneNumbers* phfwdGetReverse(PhoneForward const* pf, char const* num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers* pnum);

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const* phnumGet(PhoneNumbers const* pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */
