/** @file
 * Implementacja zadania zadania 1 (JNP).
 *
 * @author Damian Dąbrowski
 * @date 2022
 */

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define top7 7

static const string REGEX_CORRECT_VALUE = R"(0*[1-9]\d{0,7})";
static const string
REGEX_CORRECT_INPUT = string(R"(^[^\S\r\n]*(()") + REGEX_CORRECT_VALUE +
                      string(R"(([^\S\r\n]+)") + REGEX_CORRECT_VALUE +
                      string(R"()*)|(NEW[^\S\r\n]+)") + REGEX_CORRECT_VALUE +
                      string(R"()|(TOP))[^\S\r\n]*$)");
static const string REGEX_TOP = R"(^[^\S\r\n]*TOP.*$)";
static const string REGEX_NEW = R"(^[^\S\r\n]*NEW.*$)";
static const string REGEX_EMPTY_LINE = R"(\s*)";

/**
 * Struktura służąca do przekazywania w funkcjach informacji o aktualnym
 * plebiscycie.
 */
static struct votingInfo {
    /*
     * Maksymalny numer, na jaki można zagłosować
     * (0 gdy nie zostało wskazane poleceniem NEW MAX)
     */
    int32_t max = 0;
    //Top 7 z poprzedniego globalnego rankingu
    vector<int32_t> Prev_TOP;
    //Globalny TOP ranking (utwór, punkty)
    unordered_map<int32_t, uint64_t> TOP;
    //Top 7 z poprzedniego notowania
    vector<int32_t> Prev_NEW;
    //Obecny wynik notowań (utwór, głosy)
    unordered_map<int32_t, uint64_t> NEW;
    //Zbiór numerów utworów, na które nie można już oddawać głosów.
    unordered_set<int32_t> prohibited;
} votingInfo;

/** @brief Porównuje dwa utwory.
 * Sprawdza, czy utwór z @p a powinien znajdować się w rankingu głosów wyżej
 * niż głos z @p b na podstawie liczby głosów oddanych na oba utwory. W
 * przypadku gdy liczba głosów dla @p a i @p b jest taka sama, większy jest
 * głos o mniejszym numerze identyfikacyjnym.
 *
 * @param[in] a – para uporządkowana <numer utworu, liczba oddanych głosów>;
 * @param[in] b – para uporządkowana <numer utworu, liczba oddanych głosów>;
 * @return Wartość @p true, jeśli utwór z pary @a powinien znaleźć się w
 *         rankingu wyżej niż utwór z pary @p b; w przeciwnym razie wartość
 *         @p false.
 */
struct compVotes {
    bool operator()(const pair<int32_t, uint64_t>& a,
                    const pair<int32_t, uint64_t>& b) const
    {
        if (a.second == b.second) return a.first < b.first;
        return a.second > b.second;
    }
};

/** @brief Aktualizuje ranking NEW.
 * Aktualizuje obecny wynik notowań, poprzez dodanie do notowania @p NEW
 * jednego głosu dla utworu o numerze @p number.
 *
 * @param[in] number  – numer utworu, dla którego ma zostać dodany głos.
 * @param[in,out] NEW – obecny wynik notowań, mapa (utwór, głosy).
 */
static void voteNEW(const int32_t number, unordered_map<int32_t, uint64_t>& NEW)
{
    auto search = NEW.find(number);
    if (search != NEW.end()) search->second++;
    else NEW.insert({number, 1});
}

/** @brief Aktualizuje ranking TOP.
 * Aktualizuje globalny ranking (podsumowanie) wszystkich utworów, na które
 * został oddany głos.
 *
 * @param[in] summary – wynik ostatniego notowania utworów, wektor z numerami
 *                      utworów w kolejności od tych, które zajęły najwyższą
 *                      pozycję, do tych które zajęły najniższą.
 * @param[in,out] TOP – mapa zawierająca informacje o liczbie punktów
 *                      uzyskanych przez poszczególne utwory.
 */
static void voteTOP(const vector<int32_t>& summary,
                    unordered_map<int32_t, uint64_t>& TOP)
{
    for (size_t i = 0; i < summary.size(); i++) {
        auto search = TOP.find(summary[i]);
        if (search != TOP.end()) search->second += (top7 - i);
        else TOP.insert({summary[i], (top7 - i)});
    }
}

/** @brief Znajduje i wypisuje top7 danego notowania lub podsumowania.
 * Znajduje i wypisuje 7 zwycięzców danego głosowania, aktualizuje
 * ranking top7 zwycięzców z poprzedniego notowania (Prev_NEW) lub ranking top7
 * zwycięzców w poprzednim globalnym podsumowaniu (Prev_TOP).
 * Aktualizuje globalny ranking (podsumowanie) wszystkich utworów, na które
 * został oddany głos. Czyści notowanie lub podsumowania @p TOP_NEW.
 *
 * @param[in,out] TOP_NEW      – wynik notowania (mapa <numer_utworu,
 *                               liczba_głosów>) lub globalny ranking utworów
 *                               (mapa <numer_utworu, liczba_punktów>)
 * @param[in,out] Prev_TOP_NEW – wynik poprzedniego notowania lub podsumowania.
 */
static void printTOP_NEW(const unordered_map<int32_t, uint64_t>& TOP_NEW,
                         vector<int32_t>& Prev_TOP_NEW) 
{
    if (TOP_NEW.empty()) return;

    //Znajduje top7
    set<pair<int32_t, uint64_t>, compVotes> ranking;
    for (const auto& val : TOP_NEW) {
        ranking.insert(val);
        if (ranking.size() > top7)
            ranking.erase(prev(ranking.end()));
    }

    //Gotowe wyniki
    vector<int32_t> summary;
    for (const auto& val : ranking)
        summary.push_back(val.first);


    //Wypisuje
    for (size_t i = 0; i < summary.size(); i++) {
        cout << summary[i] << " ";

        auto it = find(Prev_TOP_NEW.begin(), Prev_TOP_NEW.end(), summary[i]);
        if (it != Prev_TOP_NEW.end())
            cout << (it - Prev_TOP_NEW.begin()) - (int32_t) i << "\n";
        else cout << "-\n";
    }

    //Aktualizuje Prev_TOP albo Prev_NEW
    Prev_TOP_NEW = summary;

    cout.flush();
}

/** @brief Realizuje użycie polecenia NEW MAX.
 * Znajduje i wypisuje numery siedmiu utworów, które uzyskały najwięcej
 * głosów w obecnym notowaniu wraz z informacją, o ile numerów zmieniła się
 * pozycja każdego z utworów ("-" w przypadku, gdy utwór nie występował w
 * poprzednich notowaniach). Aktualizuje rankingi @p votingInfo.TOP i @p
 * votingInfo.NEW, aktualizuje wartość @p votingInfo.max i aktualizuje listę
 * utworów, na które nie można już głosować (@p votingInfo.prohibited).
 *
 * @param[in,out] vInfo – struktura przechowująca informacje o plebiscycie.
 * @param[in] new_max        – nowa wartość MAX (najwyższy numer
 *                             identyfikacyjny utworu).
 */
static void commandNew(struct votingInfo& vInfo, const int32_t new_max)
{
    if (vInfo.NEW.empty()) {
        //zakazuje wszystkie z poprzedniego Prev_NEW
        copy(vInfo.Prev_NEW.begin(), vInfo.Prev_NEW.end(),
             inserter(vInfo.prohibited, vInfo.prohibited.end()));
        vInfo.Prev_NEW.clear();
    } else {
        //Kopia poprzednich zwycięzców
        vector <int32_t> copy_Prev = vInfo.Prev_NEW;
        //Wypisuje zwycięzców, zapisuje wyniki w Prev_NEW;
        printTOP_NEW(vInfo.NEW, vInfo.Prev_NEW);

        // Zakazane numery, które nie pojawiły się ponownie
        for (auto num : copy_Prev)
            if (find(vInfo.Prev_NEW.begin(), vInfo.Prev_NEW.end(),
                     num) == vInfo.Prev_NEW.end())
                vInfo.prohibited.insert(num);
    }

    voteTOP(vInfo.Prev_NEW, vInfo.TOP); //Aktualizuje ranking TOP
    vInfo.NEW.clear();
    vInfo.max = new_max;
}

/** @brief Sprawdza, czy oddany głos jest poprawny.
 * Sprawdza, czy numery utworów w oddanym głosie @p inputLine nie znajdują
 * się na liście utworów, na które nie można oddawać głosów, czy wskazane
 * numery utworów mieszczą się w żądanym zakresie i czy numery utworów w
 * oddanym głosie są parami różne.
 *
 * @param[in] inputLine  – linia ze standardowego wejście zawierająca
 *                         informacje o oddanym głosie.
 * @param[in] max        – maksymalny numer utworu, na jaki można oddać głos.
 * @param[in] prohibited – zbiór zawierający numery utworów, na które nie
 *                         można oddawać głosu.
 * @return Wartość @p true, jeśli oddany głos jest poprawny, @p false w
 *         przeciwnym razie.
 */
static bool isVoteCorrect(const string& inputLine, const int32_t& max,
                          const unordered_set<int32_t>& prohibited)
{
    stringstream ss;
    ss << inputLine;
    int32_t number;
    unordered_set<int32_t> castedVote;
    while (ss >> number) {
        if (number > max || prohibited.find(number) != prohibited.end() ||
                castedVote.find(number) != castedVote.end())
            return false;
        castedVote.insert(number);
    }

    return true;
}

/** @brief Sprawdza, czy format linii wejściowej jest prawidłowy.
 * Sprawdza, czy linia wejściowa może być prawidłowym poleceniem TOP, NEW lub
 * oddanym głosem.
 *
 * @param[in] inputLine  – linia ze standardowego wejście zawierająca
 *                         informacje o oddanym głosie.
 * @return Wartość @p true, jeśli format linii jest poprawny, @p false w
 *         przeciwnym razie.
 */
static bool isLineFormatCorrect(const string& inputLine)
{
    regex reg(REGEX_CORRECT_INPUT);
    return regex_match(inputLine, reg);
}

/** @brief Sprawdza, czy podana linia wejściowa jest pusta.
 * Sprawdza, czy linia wejściowa, podana jako argument funkcji, zawiera
 * jedynie białe znaki.
 *
 * @param[in] inputLine  – linia ze standardowego wejście zawierająca
 *                         informacje o oddanym głosie.
 * @return Wartość @p true, jeśli linia jest pusta, @p false w przeciwnym razie.
 */
static bool emptyLine(const string& inputLine)
{
    regex reg(REGEX_EMPTY_LINE);
    return regex_match(inputLine, reg);
}

/** @brief Obsługuje błędy.
 * Wypisuje na wyjście diagnostyczne informacje o numerze linii, w której
 * wystąpił błąd oraz oryginalną postać błędnej linii.
 *
 * @param[in] lineNumber – numer linii, w której wystąpił błąd.
 * @param[in] inputLine  – linia ze standardowego wejście zawierająca
 *                         informacje o oddanym głosie.
 */
static void handleError(const uint64_t lineNumber, const string& inputLine)
{
    cerr << "Error in line " << lineNumber << ": " << inputLine << "\n";
}

/** @brief Obsługuje kolejne linie ze standardowego wejścia.
 * Wypisuje na wyjście diagnostyczne informacje o numerze linii, w której
 * wystąpił błąd oraz oryginalną postać błędnej linii.
 *
 * @param[in] lineNumber  – zmienna przechowująca aktualny numer linii.
 * @param[in] inputLine   – linia ze standardowego wejście zawierająca
 *                          informacje o oddanym głosie.
 * @param[in,out] vInfo – struktura przechowująca informacje o plebiscycie.
 */
static void handleLine(const uint64_t lineNumber, const string& inputLine,
                       struct votingInfo& vInfo)
{
    if (isLineFormatCorrect(inputLine)) {
        regex rTop(REGEX_TOP);
        regex rNew(REGEX_NEW);

        if (regex_match(inputLine, rNew)) {
            regex newVal(REGEX_CORRECT_VALUE);
            smatch match;
            regex_search(inputLine, match, newVal);
            int32_t newMax = stoi(match.str());
            if (newMax >= vInfo.max)
                commandNew(vInfo, newMax);
            else
                handleError(lineNumber, inputLine);

        } else if (regex_match(inputLine, rTop))
            printTOP_NEW(vInfo.TOP, vInfo.Prev_TOP);
        else {
            if (isVoteCorrect(inputLine, vInfo.max,
                              vInfo.prohibited)) {
                stringstream ss;
                int32_t number;
                ss << inputLine;
                while (ss >> number)
                    voteNEW(number, vInfo.NEW);
            } else
                handleError(lineNumber, inputLine);
        }
    } else if (!emptyLine(inputLine))
        handleError(lineNumber, inputLine);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    uint64_t line_number = 1; //Obecny numer linii wejścia
    string s;
    while (getline(cin, s)) {
        handleLine(line_number, s, votingInfo);
        line_number++;
    }
    return 0;
}