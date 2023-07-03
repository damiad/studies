#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

// Ten plik włączamy jako pierwszy i dwa razy, aby sprawdzić, czy zawiera
// wszystkie potrzebne deklaracje i definicje oraz ochronę przed wielokrotnym
// włączeniem.
#include "phone_forward.h"
#include "phone_forward.h"

#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

// Gdzieś musi być zdefiniowany magiczny napis służący do spawdzania, czy
// program w całości wykonał się poprawnie.
extern char quite_long_magic_string[];
char quite_long_magic_string[] = "MAGIC";

// Możliwe wyniki testu
#define PASS 0
#define FAIL 1
#define WRONG_TEST 2
#define PASS_INSTRUMENTED 3

// Liczba elementów tablicy x
#define SIZE(x) (sizeof(x) / sizeof(x)[0])

// Początek testu
#define INIT(p)                 \
  PhoneForward *p = phfwdNew(); \
  if (p == NULL)                \
    return FAIL

// Koniec testu
#define CLEAN(p)  \
  phfwdDelete(p); \
  return PASS;

// Wypełnienie numeru znakiem
#define FILL(p, from, to, c)           \
  do {                                 \
    for (int _k = from; _k < to; ++_k) \
      p[_k] = c;                       \
    p[to] = '\0';                      \
  } while (0)

// Skopiowanie numeru
#define COPY(dst, src) \
  strcpy(dst, src)

// Oczekiwane jednakowe numery
#define C(x, y)            \
  do {                     \
    if (strcmp(x, y) != 0) \
      return FAIL;         \
  } while (0)

// Oczekiwany zerowy wynik funkcji
#define Z(f)       \
  do {             \
    if ((f) != 0)  \
      return FAIL; \
  } while (0)

// Oczekiwany wynik funkcji false
#define F(f)          \
  do {                \
    if ((f) != false) \
      return FAIL;    \
  } while (0)

// Oczekiwany niezerowy wynik funkcji
#define N(f)       \
  do {             \
    if ((f) == 0)  \
      return FAIL; \
  } while (0)

// Oczekiwany wynik funkcji true
#define T(f)         \
  do {               \
    if ((f) != true) \
      return FAIL;   \
  } while (0)

// Oczekiwany numer n jako i-ty w strukturze PhoneNumbers
#define R(p, i, n) \
  C(phnumGet(p, i), n)

// Oczekiwany brak i-tego numeru w strukturze PhoneNumbers
#define Q(p, i) \
  Z(phnumGet(p, i))

// Oczekiwana pusta struktura PhoneNumbers
#define E(f)         \
  do {               \
   PhoneNumbers *_p; \
   N(_p = f);        \
   Q(_p, 0);         \
   Q(_p, 1);         \
   Q(_p, 9999);      \
   phnumDelete(_p);  \
  } while (0)

// Oczekiwane przekierowanie z A na B
#define CHECK(p, A, B)      \
  do {                      \
    PhoneNumbers *_p;       \
    N(_p = phfwdGet(p, A)); \
    R(_p, 0, B);            \
    Q(_p, 1);               \
    phnumDelete(_p);        \
  } while (0)

/** WŁAŚCIWE TESTY **/

// Tylko utworzenie i usunięcie struktury
static int empty_struct(void) {
  INIT(pf);
  CLEAN(pf);
}

// Brak przekierowań
static int no_forward(void) {
  char n[2];

  INIT(pf);

  n[1] = '\0';
  for (n[0] = '9'; n[0] >= '0'; --n[0])
    phfwdRemove(pf, n);
  for (n[0] = '0'; n[0] <= '9'; ++n[0])
    CHECK(pf, n, n);

  CLEAN(pf);
}

// Błędne argumenty funkcji
static int wrong_arguments(void) {
  INIT(pf);

  F(phfwdAdd(pf, "123", ""));
  F(phfwdAdd(pf, "123", "a"));
  F(phfwdAdd(pf, "123", " "));
  F(phfwdAdd(pf, "123", "9b"));
  F(phfwdAdd(pf, "123", "@"));
  F(phfwdAdd(pf, "123", "/"));
  F(phfwdAdd(pf, "123", ":"));
  F(phfwdAdd(pf, "123", ";"));
  F(phfwdAdd(pf, "123", ">"));
  F(phfwdAdd(pf, "123", "0?"));
  F(phfwdAdd(pf, "123", "*"));
  F(phfwdAdd(pf, "123", "#"));
  F(phfwdAdd(pf, "", "123"));
  F(phfwdAdd(pf, "c", "123"));
  F(phfwdAdd(pf, " ", "123"));
  F(phfwdAdd(pf, "8d", "123"));
  F(phfwdAdd(pf, "/", "123"));
  F(phfwdAdd(pf, ":", "123"));
  F(phfwdAdd(pf, ";", "123"));
  F(phfwdAdd(pf, "1>", "123"));
  F(phfwdAdd(pf, "?", "123"));
  F(phfwdAdd(pf, "@", "123"));
  F(phfwdAdd(pf, "*", "123"));
  F(phfwdAdd(pf, "#", "123"));
  F(phfwdAdd(pf, "0", "0"));

  E(phfwdGet(pf, ""));
  E(phfwdGet(pf, "e"));
  E(phfwdGet(pf, " "));
  E(phfwdGet(pf, "7f"));
  E(phfwdGet(pf, "%"));
  E(phfwdGet(pf, "/"));
  E(phfwdGet(pf, "%"));
  E(phfwdGet(pf, ":"));
  E(phfwdGet(pf, ";"));
  E(phfwdGet(pf, "?"));
  E(phfwdGet(pf, ">"));
  E(phfwdGet(pf, "@"));

  Z(phfwdGet(NULL, "123"));
  Z(phfwdGet(NULL, NULL));
  Z(phfwdGet(NULL, "abc"));

  phfwdRemove(pf, "");
  phfwdRemove(pf, "g");
  phfwdRemove(pf, " ");
  phfwdRemove(pf, "6h");
  phfwdRemove(pf, "/");
  phfwdRemove(pf, "%");
  phfwdRemove(pf, ":");
  phfwdRemove(pf, ";");
  phfwdRemove(pf, ">");
  phfwdRemove(pf, "@");
  phfwdRemove(pf, "*");
  phfwdRemove(pf, "#");

  CLEAN(pf);
}

// Błędne argumenty funkcji bez kończącego zera
static int malicious_arguments(void) {
  INIT(pf);

  long page_size = sysconf(_SC_PAGE_SIZE);
  char *mem = mmap(NULL, 2 * page_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (mem == (char *)-1)
    return FAIL;
  if (munmap(mem + page_size, page_size) < 0)
    return FAIL;
  memset(mem, 'A', page_size);

  F(phfwdAdd(pf, mem, mem));
  E(phfwdGet(pf, mem));
  phfwdRemove(pf, mem);

  if (munmap(mem, page_size) < 0)
    return FAIL;

  CLEAN(pf);
}

// Niepsucie bazy przez błędne wywołania funkcji
static int breaking_struct(void) {
  char a[2], b[2];

  INIT(pf);

  a[1] = b[1] = '\0';
  for (a[0] = '0', b[0] = '9'; a[0] <= '9'; ++a[0], --b[0])
    T(phfwdAdd(pf, a, b));

  F(phfwdAdd(pf, "1", "A"));
  F(phfwdAdd(pf, "2D", "0"));
  F(phfwdAdd(pf, "B", "2"));
  phfwdRemove(pf, "C");

  CLEAN(pf);
}

// Długie numery
static int long_numbers(void) {
  #define LEN1 1000
  #define LEN2 4500
  #define LEN3 1500
  #define LEN4 5000

  char n1[LEN1 + 1], n2[LEN2 + 1], n3[LEN3 + 1], n4[LEN4 + 1];

  FILL(n1, 0, LEN1, '1');
  FILL(n2, 0, LEN1, '1');
  FILL(n2, LEN1, LEN2, '2');
  FILL(n3, 0, LEN3, '3');
  FILL(n4, 0, LEN3, '3');
  FILL(n4, LEN3, LEN4, '2');

  INIT(pf);

  T(phfwdAdd(pf, n1, n3));
  CHECK(pf, n2, n4);

  CLEAN(pf);

  #undef LEN1
  #undef LEN2
  #undef LEN3
  #undef LEN4
}

// Kopiowanie wartości do wnętrza struktury
static int copy_arguments(void) {
  #define LEN5 5

  static const char p1[] = "123";
  static const char p2[] = "456";
  char p3[LEN5 + 1], p4[LEN5 + 1];

  INIT(pf);

  COPY(p3, p1);
  COPY(p4, p2);

  T(phfwdAdd(pf, p3, p4));

  FILL(p3, 0, LEN5, '\0');
  FILL(p4, 0, LEN5, '\0');

  CHECK(pf, "12345", "45645");

  CLEAN(pf);

  #undef LEN5
}

// Dwie instancje struktury
static int two_structs(void) {
  INIT(pf1);
  INIT(pf2);

  T(phfwdAdd(pf1, "1", "5"));
  T(phfwdAdd(pf2, "2", "6"));

  CHECK(pf1, "1", "5");
  CHECK(pf1, "2", "2");
  CHECK(pf2, "1", "1");
  CHECK(pf2, "2", "6");

  phfwdDelete(pf1);
  CLEAN(pf2);
}

// Delete ze wskaźnikiem NULL
static int delete_null(void) {
  phnumDelete(NULL);
  CLEAN(NULL);
}

// Zachowanie wyniku po zmianie struktury
static int persistent_results(void) {
  PhoneNumbers *png;

  INIT(pf);

  T(phfwdAdd(pf, "3", "4"));

  N(png = phfwdGet(pf, "33"));

  T(phfwdAdd(pf, "3", "5"));
  phfwdRemove(pf, "3");
  phfwdDelete(pf);

  R(png, 0, "43");
  Q(png, 1);
  phnumDelete(png);

  return PASS;
}

// Nadpisanie przekierowania
static int forward_overwrite(void) {
  INIT(pf);

  CHECK(pf, "000", "000");

  T(phfwdAdd(pf, "0", "5"));
  CHECK(pf, "000", "500");

  T(phfwdAdd(pf, "0", "7"));
  CHECK(pf, "000", "700");

  CLEAN(pf);
}

// Usunięcie przekierowania
static int remove_forward(void) {
  INIT(pf);

  T(phfwdAdd(pf, "0", "5"));
  CHECK(pf, "000", "500");

  phfwdRemove(pf, "0");
  CHECK(pf, "000", "000");

  CLEAN(pf);
}

// Różne operacje na strukturze
static int various_ops(void) {
  INIT(pf);

  T(phfwdAdd(pf, "123", "9"));
  CHECK(pf, "1234", "94");
  T(phfwdAdd(pf, "123456", "777777"));
  CHECK(pf, "12345", "945");
  CHECK(pf, "123456", "777777");
  CHECK(pf, "997", "997");
  phfwdRemove(pf, "12");
  CHECK(pf, "123456", "123456");
  T(phfwdAdd(pf, "567", "0"));
  T(phfwdAdd(pf, "5678", "08"));
  T(phfwdAdd(pf, "12", "123"));
  CHECK(pf, "123", "1233");
  T(phfwdAdd(pf, "2", "4"));
  T(phfwdAdd(pf, "23", "4"));

  phfwdDelete(pf);
  N(pf = phfwdNew());

  T(phfwdAdd(pf, "123", "76"));
  CHECK(pf, "1234581", "764581");
  CHECK(pf, "7581", "7581");

  CLEAN(pf);
}

// Dużo operacji na strukturze
static int many_ops() {
  #define xfrom   00000
  #define xto     99999
  #define div0    1
  #define format0 "%05u"
  #define div1    10
  #define format1 "%04u"
  #define div2    100
  #define format2 "%03u"
  #define div3    1000
  #define format3 "%02u"

  char b1[16], b2[16];

  INIT(pf);

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0, i);
    sprintf(b2, format2, i / div2);
    T(phfwdAdd(pf, b1, b2));
  }

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0 "123", i);
    sprintf(b2, format2 "123", i / div2);
    CHECK(pf, b1, b2);
  }

  for (unsigned i = xfrom / div1; i <= xto / div1; ++i) {
    sprintf(b1, format1, i);
    CHECK(pf, b1, b1);
  }

  for (unsigned i = xfrom / div2; i <= xto / div2; ++i) {
    sprintf(b1, format2, i);
    phfwdRemove(pf, b1);
  }

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0, i);
    CHECK(pf, b1, b1);
  }

  CLEAN(pf);

  #undef xfrom
  #undef xto
  #undef div0
  #undef format0
  #undef div1
  #undef format1
  #undef div2
  #undef format2
  #undef div3
  #undef format3
}

// Bardzo długie numery
static int very_long(void) {
  #define LONG_LEN 250000

  char *base, b[3];

  INIT(pf);
  N(base = malloc(sizeof(char) * (LONG_LEN + 3)));

  for (int i = 0; i < LONG_LEN; ++i)
    base[i] = '0' + i % 10;
  base[LONG_LEN + 2] = '\0';
  b[2] = '\0';
  for (int i = 0; i <= 99; ++i) {
    b[0] = '0' + i / 10;
    b[1] = '0' + i % 10;
    base[LONG_LEN]     = '0' + i % 10;
    base[LONG_LEN + 1] = '0' + i / 10;
    T(phfwdAdd(pf, base, b));
  }
  for (int i = 0 ; i <= 99; ++i) {
    b[0] = '0' + i / 10;
    b[1] = '0' + i % 10;
    base[LONG_LEN]     = '0' + i % 10;
    base[LONG_LEN + 1] = '0' + i / 10;
    CHECK(pf, base, b);
  }

  free(base);
  CLEAN(pf);

  #undef LONG_LEN
}

// Duża liczba przekierowań
static int many_remove(void) {
  #define BIG_COUNT 10000

  char *base;

  INIT(pf);
  N(base = malloc(sizeof(char) * (BIG_COUNT + 1)));

  for (int i = 0; i < BIG_COUNT; ++i)
    base[i] = '0' + i % 10;
  for (int i = BIG_COUNT; i > 1; --i) {
    base[i] = '\0';
    T(phfwdAdd(pf, base, "0"));
  }
  T(phfwdAdd(pf, "123456789", "0"));
  phfwdRemove(pf, "0");

  free(base);
  CLEAN(pf);

  #undef BIG_COUNT
}

// Intensywne dodawanie i usuwanie przekierowań
static int add_remove(void) {
  #define yfrom   0000
  #define yto     9999
  #define yformat "%04u"
  #define REPEAT_COUNT 27

  char b1[8], b2[16];

  INIT(pf);

  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    T(phfwdAdd(pf, b1, b2));
  }
  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    for (unsigned j = 0; j < REPEAT_COUNT; ++j) {
      phfwdRemove(pf, b1);
      T(phfwdAdd(pf, b1, b2));
    }
  }
  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    CHECK(pf, b1, b2);
  }

  CLEAN(pf);

  #undef yfrom
  #undef yto
  #undef yformat
  #undef REPEAT_COUNT
}

/** TESTY ALOKACJI PAMIĘCI
    Te testy muszą być linkowane z opcjami
    -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc
    -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup
**/

// Przechwytujemy funkcje alokujące i zwalniające pamięć.
void *__real_malloc(size_t size) __attribute__((weak));
void *__real_calloc(size_t nmemb, size_t size)__attribute__((weak));
void *__real_realloc(void *ptr, size_t size)__attribute__((weak));
void *__real_reallocarray(void *ptr, size_t nmemb, size_t size)__attribute__((weak));
char *__real_strdup(const char *s)__attribute__((weak));
char *__real_strndup(const char *s, size_t size)__attribute__((weak));
void __real_free(void *ptr)__attribute__((weak));

// Trzymamy globalnie informacje o alokacjach i zwolnieniach pamięci i nie
// pozwalamy ich optymalizować.
static volatile unsigned call_counter = 0;  // licznik wywołań alokacji
static volatile unsigned fail_counter = 0;  // numer błędnej alokacji
static volatile unsigned alloc_counter = 0; // liczba wykonanych alokacji
static volatile unsigned free_counter = 0;  // liczba wykonanych zwolnień
static volatile char *function_name = NULL; // nazwa nieudanej funkcji
static volatile bool wrap_flag = false;     // przechwytujące funkcje były wołane

// W zadanym momencie alokacja pamięci zawodzi.
static bool should_fail(void) {
  return ++call_counter == fail_counter;
}

// Realokacja musi się udać, jeśli nie zwiększamy rozmiaru alokowanej pamięci.
static bool can_fail(void const *old_ptr, size_t new_size) {
  if (old_ptr == NULL)
    return true;
  else
    return new_size > malloc_usable_size((void *)old_ptr);
}

// Symulujemy brak pamięci.
#define UNRELIABLE_ALLOC(ptr, size, fun, name) \
  do { \
    wrap_flag = true; \
    if (ptr != NULL && size == 0) { \
      /* Takie wywołanie realloc jest równoważne wywołaniu free(ptr). */ \
      ++free_counter; \
      return fun; \
    } \
    void *p = can_fail(ptr, size) && should_fail() ? NULL : (fun); \
    if (p) { \
      alloc_counter += ptr != p; \
      free_counter += ptr != p && ptr != NULL; \
    } \
    else { \
      function_name = name; \
    } \
    return p; \
  } while (0)

void *__wrap_malloc(size_t size) {
  UNRELIABLE_ALLOC(NULL, size, __real_malloc(size), "malloc");
}

void *__wrap_calloc(size_t nmemb, size_t size) {
  UNRELIABLE_ALLOC(NULL, nmemb * size, __real_calloc(nmemb, size), "calloc");
}

void *__wrap_realloc(void *ptr, size_t size) {
  UNRELIABLE_ALLOC(ptr, size, __real_realloc(ptr, size), "realloc");
}

void *__wrap_reallocarray(void *ptr, size_t nmemb, size_t size) {
  UNRELIABLE_ALLOC(ptr, nmemb * size, __real_reallocarray(ptr, nmemb, size), "reallocarray");
}

char *__wrap_strdup(const char *s) {
  UNRELIABLE_ALLOC(NULL, 0, __real_strdup(s), "strdup");
}

char *__wrap_strndup(const char *s, size_t size) {
  UNRELIABLE_ALLOC(NULL, 0, __real_strndup(s, size), "strndup");
}

// Zwalnianie pamięci zawsze się udaje. Odnotowujemy jedynie fakt zwolnienia.
void __wrap_free(void *ptr) {
  __real_free(ptr);
  if (ptr)
    ++free_counter;
}

// Test reakcji implementacji na niepowodzenie alokacji pamięci
static unsigned alloc_fail_test(void) {
  unsigned visited = 0;
  PhoneForward *pf = NULL;
  PhoneNumbers *pn = NULL;

  if ((pf = phfwdNew()) != NULL) {
    visited |= 01;
  }
  else if ((pf = phfwdNew()) != NULL) {
    visited |= 02;
  }
  else {
    visited |= 04;
    return visited;
  }

  if (phfwdAdd(pf, "579", "4")) {
    visited |= 010;
  }
  else if (phfwdAdd(pf, "579", "4")) {
    visited |= 020;
  }
  else {
    visited |= 040;
    phfwdDelete(pf);
    return visited;
  }

  if ((pn = phfwdGet(pf, "5791")) != NULL) {
    visited |= 0100;
  }
  else if ((pn = phfwdGet(pf, "5791")) != NULL) {
    visited |= 0200;
  }
  else {
    visited |= 0400;
    phfwdDelete(pf);
    return visited;
  }

  phnumDelete(pn);
  phfwdDelete(pf);
  return visited;
}

// Sprawdzenie reakcji implementacji na niepowodzenie alokacji pamięci
static int alloc_fail(void) {
  unsigned fail, pass;
  for (fail = 0, pass = 0, fail_counter = 1; fail < 3 && pass < 3; ++fail_counter) {
    call_counter = 0;
    alloc_counter = 0;
    free_counter = 0;
    function_name = NULL;
    unsigned visited_point = alloc_fail_test();

    if (alloc_counter != free_counter || (visited_point & 04444444444) != 0) {
      fprintf(stderr,
              "fail_counter %u, alloc_counter %u, free_counter %u, "
              "function_name %s, visited_point %o\n",
              fail_counter, alloc_counter, free_counter,
              function_name, visited_point);
      ++fail;
    }
    if (function_name == NULL)
      ++pass;
    else
      pass = 0;
  }
  if (wrap_flag && fail == 0)
    return PASS_INSTRUMENTED;
  else if (!wrap_flag && fail == 0)
    return PASS;
  else
    return FAIL;
}

/** URUCHAMIANIE TESTÓW **/

typedef struct {
  char const *name;
  int (*function)(void);
} test_list_t;

#define TEST(t) {#t, t}

static const test_list_t test_list[] = {
  TEST(empty_struct),
  TEST(no_forward),
  TEST(wrong_arguments),
  TEST(malicious_arguments),
  TEST(breaking_struct),
  TEST(long_numbers),
  TEST(copy_arguments),
  TEST(two_structs),
  TEST(delete_null),
  TEST(persistent_results),
  TEST(forward_overwrite),
  TEST(remove_forward),
  TEST(various_ops),
  TEST(many_ops),
  TEST(very_long),
  TEST(many_remove),
  TEST(add_remove),
  TEST(alloc_fail),
};

static int do_test(int (*function)(void)) {
  int result = function();
  puts(quite_long_magic_string);
  return result;
}

int main(int argc, char *argv[]) {
  if (argc == 2)
    for (size_t i = 0; i < SIZE(test_list); ++i)
      if (strcmp(argv[1], test_list[i].name) == 0)
        return do_test(test_list[i].function);

  fprintf(stderr, "Użycie:\n%s nazwa_testu\n", argv[0]);
  return WRONG_TEST;
}
