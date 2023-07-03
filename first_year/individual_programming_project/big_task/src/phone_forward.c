/** @file
 * Moduł klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Damian Dąbrowski <dd439954@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Przedstwia pojedynczy numer (bądź prefix pewnego numeru).
 */
struct PhoneNumber {
    size_t digits; ///< Ilość znaków bez null byte
    char* number;  ///< Ciąg cyfr (znaków)
};
typedef struct PhoneNumber PhoneNumber; ///< Pojedynczy umer jako PhoneNumber

/**
 * Przechowuje przekierownaia numerów telefonów.
 * Z numeru zapisanego we form pod indeksem i,
 * na numer zpisany w to, pod indeksem i.
 */
struct PhoneForward {
    size_t rules_number; ///< Ile różnych przekierowań
    size_t buffers_size; ///< Na ile numerów zaallokowana pamięć
    PhoneNumber* from;   ///< Numery z których przekierowujemy
    PhoneNumber* to;     ///< Numery na które są przekierowania
};
typedef struct PhoneForward PhoneForward; ///< Przekierowania jako PhoneForward

/**
 * Przechowuje ciąg numerów telefonów.
 */
struct PhoneNumbers {
    size_t how_many_numbers; ///< Ile różnych numerów
    size_t buffer_size;      ///< Na ile numerów zaallokowana pamięć
    PhoneNumber* num;        ///< Kolejne numery
};
typedef struct PhoneNumbers PhoneNumbers; ///< Ciąg numerów jako PhoneNumbers

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward* phfwdNew(void)
{
    PhoneForward* pf = malloc(sizeof(PhoneForward));
    if (pf != NULL) {
        pf->rules_number = 0;
        pf->buffers_size = 0;
        pf->from = NULL;
        pf->to = NULL;
    }

    return pf;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward* pf)
{
    if (pf != NULL) {
        for (size_t i = 0; i < pf->rules_number; i++) {
            free(pf->from[i].number);
            free(pf->to[i].number);
        }
        free(pf->from);
        free(pf->to);
        free(pf);
    }
}

/** @brief Sprawdza, czy znak jest cyfrą.
 * Sprawdza, czy znak, reprezentowany przez @p c reprezentuje cyfrę numeru telefonu.
 * To znaczy: cyfrę od 0 do 9, * lub #.
 * @param[in] c – znak typu char.
 * @return Wartość @p true, jeśli to dozwolony znak numeru telefonu.
 *         Wartość @p false, jeśli znak nie jest dozwolony w numerze telefonu.
 */
static bool isitdigit(char c)
{
    if (c == '*' || c == '#') return true;
    return isdigit(c);
}

/** @brief Sprawdza, czy string reprezentuje liczbę.
 * Sprawdza, czy ciąg na, który wskazuje @p num reprezentuje numer telefonu.
 * Znajduje, jego długość i ją zwraca.
 * @param[in] num – wskaźnik na ciąg znaków.
 * @return Wartość @p >0, jeśli to numer telefonu.
 *         Wartość @p 0, jeśli podany napis nie
 *         reprezentuje numeru.
 */
static size_t isnumber(char const* num)
{

    if (!num)
        return 0;      // ciąg pusty
    size_t length = 0; // ilość cyfr

    for (; isitdigit(num[length]); length++)
        ; // dopóki cyfra to zwiększmay ilość cyfr

    if (num[length] != '\0')
        length = 0; // znak to nie cyfra ani nullbyte

    return length; // same cyfry i '\0' na końcu
}

/** @brief Porównuje numery i zwraca znak wyniku porównania.
 * Porównuje kolejne znaki numerów, aż napotka pierwszą różnicę.
 * Zwraca liczbę dodatnią jeśli @p num1 jest większy od @p num2.
 * Jak napisy są identyczne zwraca 0. Zwraca liczbę ujemną w pozostałym przypadku.
 * Zastosowana konwencja, że 0<1<2<3<4<5<6<7<8<9<*<#.
 * @param[in] num1 – wskaźnik na napis reprezentujący pierwszy  numer
 *                   (bądź jego prefiks);
 * @param[in] num2 – wskaźnik na napis reprezentujący drugi numer
 *                   (bądź jego prefiks).
 * @return @p >0 gdy @p num1 > @p num2.
 *         @p <0 gdy @p num1 < @p num2.
 *         @p =0 gdy @p num1 = @p num2.
 */
static int num_compare(char const* num1, char const* num2)
{
    while (*num1 != '\0' && (*num1 == *num2)) {
        num1++;
        num2++;
    }
    if (*num1 == '#') return 1; //num1 większy
    if (*num2 == '#') return -1; //num1 mniejszy
    if (*num1 == '*') return 1; //num1 większy
    if (*num2 == '*') return -1; //num1 mniejszy
    return strcmp(num1, num2);

}

/** @brief Zwraca index pierwszego numeru >= danemu ciągowi.
 * Zwraca index pierwszego nuberu z ciągu @p numbers, takiego że
 * jest on nie mniejszy leksykograficznie niż numer @p num.
 * Jeśli @p numbers count to 0, zwraca 0.
 * @param[in] numbers – wskaźnik na strukturę zawierającą ciąg numerów;
 * @param[in] numbers_count – ilość numerów w ciąg @p numbers;
 * @param[in] num – wskaźnik na napis reprezentujący numer (bądź jego
 *                      prefiks).
 * @return Index numeru leksykograficznie >= @p num.
 *         0 jeśli ciąg numerów @p numbers jest pusty.
 */
static size_t find_index(PhoneNumber const* numbers, size_t numbers_count,
                         char const* num)
{
    if (numbers_count == 0)
        return 0;
    size_t l = 0;
    size_t r = numbers_count;
    size_t mid;
    int x;
    while (l < r) {
        mid = (l + r) / 2; // mid < numbers_count
        x = num_compare(num, numbers[mid].number);
        if (x == 0)
            return mid;
        if (x < 0)
            r = mid;
        if (x > 0)
            l = mid + 1;
    }
    return l; // l=r=mid;
}

/** @brief Allokuje pamięć na nowy numer.
 * Allokuje w @p arr miejsce na kolejne numery.
 * Jeżeli się nie powiodło zwraca fałsz.
 * @param[in] arr – wskaźnik na strukturę do powiększenia;
 * @param[in] length – rozmiar reallokacji pamięci.
 * @return Wartość @p true, jeśli struktra została powiększona.
 *         Wartość @p false, jeśli nie udało
 *         się alokować pamięci.
 */
static bool check_realloc(PhoneNumber** arr, size_t length)
{

    PhoneNumber* tmp = realloc(*arr, (2 * length + 1) * sizeof(PhoneNumber));
    // przypadek length==0 rozwiązanay
    if (tmp == NULL)
        return false;
    *arr = tmp;
    return true;
}

/** @brief Wstawia przekierowanie.
 * Wstawia przekierowanie z @p num1 na @p num2 do struktury @p pf.
 * Zwraca fałsz gdy operacja się nie powiodła, bo zabrakło pamięci.
 * @param[in, out] pf – wskaźnik na strukturę do której należy wstawić
 * przekierowanie;
 * @param[in] position – index na który przekierowanie ma zostać dodane;
 * @param[in] num1 – wskaźnik na prefix numeru z którego jest przekierowanie;
 * @param[in] num1_length - ilość cyfr prefixu numeru z którego jest
 * przekierowanie;
 * @param[in] num2 – wskaźnik na prefix numeru na który jest przekierowanie;
 * @param[in] num2_length - ilość cyfr prefixu numeru na który jest
 * przekierowanie.
 * @return Wartość @p true, jeśli udało się dodać przekierowanie.
 *         Wartość @p false, jeśli nie udało się dodać przekierowania,
 *         bo zabrakło pamięci.
 */
static bool put_on_position(PhoneForward* pf, size_t position, char const* num1,
                            size_t num1_length, char const* num2, size_t num2_length)
{

    if (pf->rules_number == pf->buffers_size) {
        if (!check_realloc(&pf->from, pf->buffers_size))
            return false;
        if (!check_realloc(&pf->to, pf->buffers_size))
            return false;
        pf->buffers_size = 2 * pf->buffers_size + 1; // udało się powiększyć
    }

    PhoneNumber number1;
    number1.number = calloc(num1_length + 1, sizeof(char)); // na końcu damy
    // '\0'
    number1.digits = num1_length;
    if (number1.number == NULL)
        return false;
    memcpy(number1.number, num1, num1_length * sizeof(char));

    PhoneNumber number2;
    number2.number = calloc(num2_length + 1, sizeof(char));
    number2.digits = num2_length;
    if (number2.number == NULL) {
        free(number1.number);
        return false;
    }
    memcpy(number2.number, num2, num2_length * sizeof(char));

    pf->rules_number++;
    for (size_t i = pf->rules_number - 1; i > position; i--) {
        pf->from[i] = pf->from[i - 1];
        pf->to[i] = pf->to[i - 1];
    }

    pf->from[position] = number1;
    pf->to[position] = number2;

    return true;
}

/** @brief Aktualizuje dane przekierowanie.
 * Aktualizuje przekierowanie z @p pf zapisane pod indexem @p position.
 * Jeśli operacja się powiedzie to niezmieniony prefix będzie przekierowywał
 * na numer reprezentowany przez @p num (zamienia istniejące przekierowanie).
 * Zwraca fałsz gdy operacja się nie powiodła, bo zabrakło pamięci.
 * @param[in, out] pf – wskaźnik na strukturę w której należy zaktualizować
 * przekierowanie;
 * @param[in] position – index na który przekierowanie ma zostać zaktualizowane;
 * @param[in] num – wskaźnik na prefix numeru na który jest przekierowanie;
 * @param[in] num_length - ilość cyfr prefixu numeru na który jest
 * przekierowanie.
 * @return Wartość @p true, jeśli udało się zaktualizować przekierowanie.
 *         Wartość @p false, jeśli nie udało się zaktalizować przekierowania,
 *         bo zabrakło pamięci.
 */
static bool change_to_on_position(PhoneForward* pf, size_t position,
                                  char const* num,
                                  size_t num_length)
{

    PhoneNumber number;
    number.number = calloc(num_length + 1, sizeof(char));
    number.digits = num_length;
    if (number.number == NULL)
        return false;
    memcpy(number.number, num, num_length * sizeof(char));

    free(pf->to[position].number);
    pf->to[position] = number;

    return true;
}

/**
 * Sprawdza, czy @p pre jest prefiksem @p str.
 */
static bool isprefix(const char* pre, const char* str, size_t pre_length)
{
    return strncmp(pre, str, pre_length) == 0;
}

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
bool phfwdAdd(PhoneForward* pf, char const* num1, char const* num2)
{
    if (pf == NULL || num1 == NULL || num2 == NULL)
        return false;

    size_t num1_length;
    size_t num2_length;
    if (!(num1_length = isnumber(num1)))
        return false; // num1 to nie numer
    if (!(num2_length = isnumber(num2)))
        return false; // num2 to nie numer

    if (num_compare(num1, num2) == 0)
        return false; // numery identyczne

    size_t position = find_index(pf->from, pf->rules_number,
                                 num1); // znajduje gdzie wstawić
    // numer;

    if (position ==
            pf->rules_number) // wstawiamy na koniec Również przypadek
        // pf->rules_number==0 mamy z głowy bo position==0
        return put_on_position(pf, position, num1, num1_length, num2,
                               num2_length);

    if (num_compare(pf->from[position].number, num1) ==
            0) { // aktualizujemy przekierowanie (bo istnieje z tego prefixu) ->to;
        return change_to_on_position(pf, position, num2,
                                     num2_length); // false to zabrakło pamięci;
    } else { // wstawiamy na pozycję i przesuwamy strukturę
        return put_on_position(pf, position, num1, num1_length, num2,
                               num2_length); // false to zabrakło pamięci;
    }
}

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru lub @p pf to NULL, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward* pf, char const* num)
{
    if (pf == NULL || num == NULL)
        return;
    if (pf->rules_number == 0)
        return; // nie ma przekierowań

    size_t num_length;
    if (!(num_length = isnumber(num)))
        return; // num to nie numer

    size_t k = find_index(
                   pf->from, pf->rules_number,
                   num); // Zwróciło pierwszy wyraz większy równy,
    //(wszystkie wcześniejsze są mniejsze, zatem nie są prefiksami)
    size_t copy_k = k;

    while (k < pf->rules_number &&
            isprefix(num, pf->from[k].number, num_length)) { // czy prefix
        free(pf->from[k].number);
        free(pf->to[k].number);
        k++;
    }

    size_t count = k - copy_k; // ile wyrzuciliśmy elementów
    for (size_t i = copy_k + count; i < pf->rules_number;
            i++) { // cofamy o count pozycji w tył
        pf->from[i - count] = pf->from[i];
        pf->to[i - count] = pf->to[i];
    }
    pf->rules_number -= count; // wyrzucone wskaźniki juz przepisane
    if (pf->buffers_size > 4 && pf->rules_number < pf->buffers_size / 3) {
        pf->buffers_size =
            pf->rules_number; // Unikamy ciągłego zmieniania rozmiaru.
        pf->from = realloc(pf->from, pf->buffers_size * sizeof(PhoneNumber));
        pf->to = realloc(pf->to, pf->buffers_size * sizeof(PhoneNumber));
    }
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers* pnum)
{
    if (pnum != NULL) {
        for (size_t i = 0; i < pnum->how_many_numbers; i++) {
            free(pnum->num[i].number); // jak to NULL to nic nie robimy...
        }
        free(pnum->num);
        free(pnum);
    }
}

/** @brief Dodaje numer do ciągu numerów, gdy jest przeciwobrazem @ref phfwdGet
 * Odszyfrowuje przekerowanie, gdzie przekierowanie prefiksu jest zapisane
 * pod indeksem @p k w struktórze @p num_from, natomiast numer wejściowy to @p num.
 * Sprawdza czy wywołanie @ref phfwdGet z tym przekierowaniem da numer @pnum.
 * Jeśli tak, to zapisuje to przekierowanie w @p pnum. Gdy zabrakło pamięci zwalnia @p pnum.
 * Zwraca true gdy dodawanie się powiodło, bądź to nie przeciwobraz, false, gdy zabrakło pamięci.
 * @param[in] pf – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num_from – ciąg prefiksów przekierowywanych;
 * @param[in] num_to – ciąg prefiksów na jakie jest przekierowanie;
 * @param[in] k – index pod którm zapisane jest przekierowanie najdłuższego
 * prefixu @p num;
 * @param[in] num – wskaźnik na numer, który jest przekierowywany;
 * @param[in] num_length - ilość cyfr numer, który jest przekierowywany;
 * @param[in, out] pnum - wskaźnik na strukturę w której docelowo ma być
 *                       zapisany wynik przekierowania.
 * @return Wartość @p true, jeśli dodawanie się powiodło, bądź to nie przeciwobraz.
 *         Wartość @p false, jeśli zabrakło pamięci.
 */
static bool create_phone_counterimage(PhoneForward const* pf,
                                      PhoneNumber* num_from,
                                      PhoneNumber* num_to, size_t k,
                                      char const* num,
                                      size_t num_length, PhoneNumbers* pnum)
{

    size_t length = num_length - num_from[k].digits + num_to[k].digits;

    PhoneNumber number1;
    number1.number = calloc(length + 1, sizeof(char)); //+1 dla nullbyte '\0'
    if (number1.number == NULL) {
        phnumDelete(pnum);
        return false; // zabrakło pamięci
    }

    memcpy(number1.number, num_to[k].number, num_to[k].digits * sizeof(char));
    memcpy(number1.number + num_to[k].digits, num + num_from[k].digits,
           (length - num_to[k].digits) * sizeof(char));

    number1.digits = length; // pomijamy nullbyte w długości w naszej strukturze

    //sprawdzamy czy to odwrotność phfwdGet
    size_t position = find_index(pf->from, pf->rules_number,
                                 number1.number);
    if (position == pf->rules_number)
        position--; // pf->rules_number>=1 czyli position będzie >=0

    while (position > 0 &&
            !isprefix(pf->from[position].number,
                      number1.number,
                      pf->from[position].digits))
        position--;

    if (k != position) { //nie dodajemy przekierowania
        free(number1.number);
        return true;
    }

    position = find_index(pnum->num, pnum->how_many_numbers, number1.number);
    if (position == pnum->how_many_numbers) {
        pnum->num[position] = number1;
        pnum->how_many_numbers++;
        return true;
    }

    pnum->how_many_numbers++;
    for (size_t i = pnum->how_many_numbers - 1; i > position; i--) {
        pnum->num[i] = pnum->num[i - 1]; //przesuwamy, żeby mieć posortowane
    }

    pnum->num[position] = number1;

    return true;
}

/** @brief Dodaje numer do ciągu numerów.
 * Odszyfrowuje przekerowanie, gdzie przekierowanie prefiksu jest zapisane
 * pod indeksem @p k w struktórze @p num_from, natomiast numer wejściowy to @p num.
 * Zapisuje to przekierowanie w @p pnum. Gdy zabrakło pamięci zwalnia @p pnum.
 * Zwraca true gdy dodawanie się powiodło, false, gdy zabrakło pamięci.
 * @param[in] num_from – ciąg prefiksów przekierowywanych;
 * @param[in] num_to – ciąg prefiksów na jakie jest przekierowanie;
 * @param[in] k – index pod którm zapisane jest przekierowanie najdłuższego
 * prefixu @p num;
 * @param[in] num – wskaźnik na numer, który jest przekierowywany;
 * @param[in] num_length - ilość cyfr numer, który jest przekierowywany;
 * @param[in, out] pnum - wskaźnik na strukturę w której docelowo ma być
 *                       zapisany wynik przekierowania.
 * @return Wartość @p true, jeśli dodawanie się powiodło.
 *         Wartość @p false, jeśli zabrakło pamięci.
 */
static bool create_phone(PhoneNumber* num_from,
                         PhoneNumber* num_to, size_t k,
                         char const* num,
                         size_t num_length, PhoneNumbers* pnum)
{

    size_t length = num_length - num_from[k].digits + num_to[k].digits;

    PhoneNumber number1;
    number1.number = calloc(length + 1, sizeof(char)); //+1 dla nullbyte '\0'
    if (number1.number == NULL) {
        phnumDelete(pnum);
        return false; // zabrakło pamięci
    }

    memcpy(number1.number, num_to[k].number, num_to[k].digits * sizeof(char));
    memcpy(number1.number + num_to[k].digits, num + num_from[k].digits,
           (length - num_to[k].digits) * sizeof(char));

    number1.digits = length; // pomijamy nullbyte w długości w naszej strukturze

    size_t position = find_index(pnum->num, pnum->how_many_numbers, number1.number);
    if (position == pnum->how_many_numbers) {
        pnum->num[position] = number1;
        pnum->how_many_numbers++;
        return true;
    }
    if (num_compare(pnum->num[position].number,
                    number1.number) == 0) { //już istnieje
        free(number1.number);
        return true; //nie dodajemy ponownie tego samego
    }

    pnum->how_many_numbers++;
    for (size_t i = pnum->how_many_numbers - 1; i > position; i--) {
        pnum->num[i] = pnum->num[i - 1]; //przesuwamy, żeby mieć posortowane
    }

    pnum->num[position] = number1;

    return true;
}

/** @brief Dodaje numer @p num do ciągu numerów.
 * Wstawia numer to @p num to struktury @p pnum.
 * Gdy zabrakło pamięci zwalnia @p pnum.
 * Zwraca true gdy dodawanie się powiodło, false, gdy zabrakło pamięci.
 * @param[in] num – wskaźnik na numer, który ma być dopisany do struktury @p
 * pnum;
 * @param[in] num_length - ilość cyfr numer, który ma być dopisany do struktury
 * @p pnum;
 * @param[in, out] pnum - wskaźnik na strukturę w której docelowo ma być
 *                       zapisany wynik dany numer telefonu.
 * @return Wartość @p true, jeśli dodawanie się powiodło.
 *         Wartość @p false, jeśli zabrakło pamięci.
 */
static bool copy_phone(char const* num, size_t num_length,
                       PhoneNumbers* pnum)
{

    PhoneNumber number1;
    number1.number = calloc(num_length + 1, sizeof(char)); //+1 za nullbyte

    if (number1.number == NULL) {
        phnumDelete(pnum);
        return false;
    }
    memcpy(number1.number, num, num_length * sizeof(char));
    number1.digits = num_length;


    size_t position = find_index(pnum->num, pnum->how_many_numbers, number1.number);
    if (position == pnum->how_many_numbers) {
        pnum->num[position] = number1;
        pnum->how_many_numbers++;
        return true;
    }
    if (num_compare(pnum->num[position].number,
                    number1.number) == 0) { //już istnieje
        return true; //nie dodajemy ponownie tego samego
    }

    pnum->how_many_numbers++;
    for (size_t i = pnum->how_many_numbers - 1; i > position; i--) {
        pnum->num[i] = pnum->num[i - 1]; //przesuwamy, żeby mieć posortowane
    }

    pnum->num[position] = number1;

    return true;
}

/** @brief Zwraca strukturę zawierającą przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Zapisuje to przekierowanie w @p pnum. Gdy zabrakło pamięci zwalnia @p pnum.
 * Zwraca @p pnum lub NULL gdy zabrakło pamięci.
 * @param[in] pf – wskaźnik na strukturę zawierającą przekierowania;
 * @param[in] num – wskaźnik na numer, który ma zostać przekierowywany;
 * @param[in] num_length - ilość cyfr numer, który ma zostać przekierowywany;
 * @param[in, out] pnum - wskaźnik na strukturę w której docelowo ma być
 *                       zapisany wynik przekierowania.
 * @return Wskaźnik na strukturę przechowującą jednoelemtowy ciąg numerów lub
 * NULL, gdy nie udało się alokować pamięci.
 */
static PhoneNumbers* redirect(PhoneForward const* pf, char const* num,
                              size_t num_length, PhoneNumbers* pnum)
{

    if (pf->rules_number == 0) {
        // zwraca ten sam numer, bo brak przekierowań
        if (! copy_phone(num, num_length, pnum)) return NULL;
        return pnum;
    }


    size_t k = find_index(pf->from, pf->rules_number, num);

    if (k == pf->rules_number)
        k--; // pf->rules_number>=1 czyli k będzie >=0

    // Prefix przekierowania musi być mniejszy leksykograficznie
    // od numeru który docelowo ma przekierować
    while (k > 0) {
        if (isprefix(pf->from[k].number, num, pf->from[k].digits)) {
            if (! create_phone(pf->from, pf->to, k, num, num_length, pnum)) return NULL;
            return pnum;
        }
        if (pf->from[k].number[0] < num[0]) {
            // zwraca ten sam numer, bo brak przekierowań
            if (!copy_phone(num, num_length, pnum)) return NULL;
            return pnum;

        }
        k--;
    }

    if (isprefix(pf->from[k].number, num, pf->from[k].digits)) { //k==0
        if (! create_phone(pf->from, pf->to, k, num, num_length, pnum)) return NULL;
        return pnum;
    }

    // zwraca ten sam numer, bo brak przekierowań
    if (!copy_phone(num, num_length, pnum)) return NULL;
    return pnum;
}

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
PhoneNumbers* phfwdGet(PhoneForward const* pf, char const* num)
{
    if (pf == NULL)
        return NULL;

    PhoneNumbers* pnum = malloc(sizeof(PhoneNumbers));
    if (pnum == NULL)
        return pnum; // zabrakło pamięci

    pnum->how_many_numbers = 0;
    pnum->buffer_size = 0;
    pnum->num = NULL;

    if (num == NULL)
        return pnum;

    size_t num_length;
    if (!(num_length = isnumber(num)))
        return pnum; // num to nie numer

    pnum->num = malloc(sizeof(PhoneNumber));
    if (pnum->num == NULL) {
        free(pnum);
        return NULL;
    }
    pnum->buffer_size = 1;

    return redirect(pf, num, num_length, pnum);
}


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
PhoneNumbers* phfwdReverse(PhoneForward const* pf, char const* num)
{
    if (pf == NULL)
        return NULL;

    PhoneNumbers* pnum = malloc(sizeof(PhoneNumbers));
    if (pnum == NULL)
        return pnum; // zabrakło pamięci

    pnum->how_many_numbers = 0;
    pnum->buffer_size = 0;
    pnum->num = NULL;

    if (num == NULL)
        return pnum;

    size_t num_length;
    if (!(num_length = isnumber(num)))
        return pnum; // num to nie numer

    for (size_t i = 0; i < pf->rules_number; i++) {
        if (isprefix(pf->to[i].number, num, pf->to[i].digits)) {
            if (pnum->how_many_numbers == pnum->buffer_size) {
                if (!check_realloc(&pnum->num, pnum->buffer_size)) {
                    phnumDelete(pnum); // zabrakło pamięci
                    return NULL;
                }
                pnum->buffer_size = 2 * pnum->buffer_size + 1; // udało się powiększyć
            }
            //odwrotnie niż w Get
            if (!create_phone(pf->to, pf->from, i, num, num_length, pnum)) return NULL;
        }
    }

    //wstawiamy numer wejściowy do ciągu
    if (pnum->how_many_numbers == pnum->buffer_size) {
        PhoneNumber* tmp = realloc(pnum->num,
                                   (pnum->how_many_numbers + 1) * sizeof(PhoneNumber));
        if (tmp == NULL) {
            phnumDelete(pnum);
            return NULL;
        }
        pnum->num = tmp;
        pnum->buffer_size++;
    }

    if (!copy_phone(num, num_length, pnum)) return NULL;
    return pnum;
}

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
PhoneNumbers* phfwdGetReverse(PhoneForward const* pf, char const* num)
{
    if (pf == NULL)
        return NULL;

    PhoneNumbers* pnum = malloc(sizeof(PhoneNumbers));
    if (pnum == NULL)
        return pnum; // zabrakło pamięci

    pnum->how_many_numbers = 0;
    pnum->buffer_size = 0;
    pnum->num = NULL;

    if (num == NULL)
        return pnum;

    size_t num_length;
    if (!(num_length = isnumber(num)))
        return pnum; // num to nie numer

    for (size_t i = 0; i < pf->rules_number; i++) {
        if (isprefix(pf->to[i].number, num, pf->to[i].digits)) {
            if (pnum->how_many_numbers == pnum->buffer_size) {
                if (!check_realloc(&pnum->num, pnum->buffer_size)) {
                    phnumDelete(pnum); // zabrakło pamięci
                    return NULL;
                }
                pnum->buffer_size = 2 * pnum->buffer_size + 1; // udało się powiększyć
            }
            //jak zabraknie pamięci to false.
            if (!create_phone_counterimage(pf, pf->to, pf->from, i, num, num_length,
                                           pnum)) return NULL;
        }
    }

    //Sprawdzamy czy wstawiamy numer wejściowy do ciągu
    if (pnum->how_many_numbers == pnum->buffer_size) {
        PhoneNumber* tmp = realloc(pnum->num,
                                   (pnum->how_many_numbers + 1) * sizeof(PhoneNumber));
        if (tmp == NULL) {
            phnumDelete(pnum);
            return NULL;
        }
        pnum->num = tmp;
        pnum->buffer_size++;
    }
    if (pf->rules_number == 0) {
        if (!copy_phone(num, num_length, pnum)) return NULL;
        return pnum; //przekierowany tylko sam na siebie
    }
    size_t position = find_index(pf->from, pf->rules_number, num);
    if (position == pf->rules_number)
        position--; // pf->rules_number>=1 czyli position będzie >=0

    bool isredirected = false;
    while (!isredirected) {
        if (isprefix(pf->from[position].number, num, pf->from[position].digits))
            isredirected = true;
        else {
            if (pf->from[position].number[0] < num[0] || position == 0) {
                if (!copy_phone(num, num_length, pnum)) return NULL;
                return pnum; //przekierowany sam na siebie
            }
        }
        position--;
    }

    return pnum;
}

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const* phnumGet(PhoneNumbers const* pnum, size_t idx)
{
    char* number = NULL;
    if (pnum == NULL)
        return number;
    if (idx >= pnum->how_many_numbers)
        return number;

    number = pnum->num[idx].number;

    return number;
}