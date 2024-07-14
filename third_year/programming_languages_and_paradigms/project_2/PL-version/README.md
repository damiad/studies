# OSkell - język programowania

## Opis

OSkell to prosty język funkcyjny, który głównie czerpie inspirację z Haskell'a i OCaml'a. Posiada silne i statyczne typowanie oraz wiele cech charakterystycznych dla obu tych języków, takich jak podobny syntax dla list, notacji funkcji czy wyrażeń `let`.
Jego dokładne cechy są przedstawione poniżej:


### Wymagania na 20 punktów:

| Wymaganie | Opis                                    | Status |
|-----------|-----------------------------------------|--------|
| 01        | Dwa typy                                | ✅     |
| 02        | Arytmetyka, porównania                  | ✅     |
| 03        | If                                      | ✅     |
| 04        | Funkcje wieloargumentowe, rekursja      | ✅     |
| 05        | Funkcje anonimowe, częściowa aplikacja  | ✅     |
| 06        | Obsługa błędów wykonania                | ✅     |
| 07        | Statyczne wiązanie identyfikatorów      | ✅     |
| 09        | Listy z empty, head, tail               | ✅     |
| 10        | Listy z lukrem                          | ✅     |

### Wymagania na 25 punktów:

| Wymaganie | Opis                                                      | Status |
|-----------|-----------------------------------------------------------|--------|
| 11        | Listy dowolnego typu, zagnieżdżone i listy funkcji        | ✅     |
| 13        | Statyczne typowanie                                       | ✅     |

### Razem: 25 punktów

## Gramatyka

Gramatyka języka znajduje się w pliku `OSkell.cf` w formacie EBNF.

## Przykłady

W katalogu `examples` są przykłady wykorzystujące opisane powyżej cechy. Katalog ten zawiera dwa podkatalogi:

- **good**: Zawiera przykłady poprawnego użycia, demonstrujące, jak interpreter radzi sobie z różnymi scenariuszami.
- **bad**: Zawiera przykłady niepoprawnego użycia, pokazujące przypadki, w których interpreter wyłapuje błędy.

Aby uruchomić przykłady, należy wykonać następujące polecenia:

```bash
make
./run-examples.sh
```
