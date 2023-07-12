global inverse_permutation
; Dane wejściowe:
; n, w rejestrze rdi - liczba elementów tablicy p
; p, w rejestrze rsi - tablica.
; Modyfikujemy rax, rdx, ecx.
; Wynik zwracany w al (0 - false, 1 - true, gdy udało się odwrócić permutację).

section .text
; Jeśli tablica wskazywana przez p zawiera permutację liczb z przedziału od 0 do n-1, 
; to funkcja odwraca tę permutację w miejscu, a wynikiem funkcji jest true. 
; W przeciwnym przypadku wynikiem funkcji jest false, a zawartość tablicy wskazywanej 
; przez p po zakończeniu wykonywania funkcji jest taka sama jak w momencie jej wywołania. 
; Funkcja wykrywa ewidentnie niepoprawne wartości n (0 lub większe niż INT_MAX + 1).
inverse_permutation:
  mov rdx, 2147483648          ; Ustawiamy wartość INT_MAX + 1 w rejestrze RDX.
  cmp rdi, rdx                 ; Sprawdzamy czy n jest w dozwolonym zakresie (1 <= n <= INT_MAX + 1).
  ja .return_false             
  cmp edi, 0                   ; Starsze 32 bity rejestru RDI są równe 0.
  je .return_false             

  xor eax, eax                 

; Sprawdza, czy liczby w tablicy p są w zakresie 0..n-1.
; Ponieważ n jest nie większe niż INT_MAX + 1, to mieści się w rejestrze 32-bitowym EDI.
; EAX jest indeksem pętli.
.check_range:
  cmp DWORD [rsi + rax*4], edi ; Warunek p[i]>=n || czy p[i]<0.
  jae .return_false
  inc eax                       
  cmp eax, edi                 
  jb .check_range                            

; Sprawdza czy liczby w tablicy p są parami rózne, czyli czy nie ma różnych 
; indeksów i, j takich, że p[i]==p[j]. Liczby w tablicy p są nieujemne, więc możemy
; użyć not p[k], aby zaznaczyć, że dla liczby k istnieje i takie, że p[i]==k.
; EAX jest indeksem pętli - i, EDX przechowuje wartość p[i].
.check_repetition:
  dec eax                      ; Na początku pętli EAX było równe n.
  mov edx, DWORD [rsi + rax*4] 
  cmp edx, 0                   ; Jak ujemne, to musimy użyć not. Aby móc
  jge .skip_not_neg            ; odwołac się do oryginalnej wartości.
  not edx                      
.skip_not_neg:
  cmp DWORD [rsi + rdx*4], 0   ; Gdy p[p[i]] < 0, to powtórka istaniało j>i
  jl .reverse_changes          ; takie, że p[i]==p[j]. Cofamy zmiany w tablicy p.
  not DWORD [rsi + rdx*4]      ; Oznaczamy, że liczba k, wystąpiła raz w tablicy p.                      
  cmp eax, 0               
  ja .check_repetition                      

; Odwraca tablicę p, przechodząc po cyklach. Tablica p zawiera negacje liczby od 0 do n-1.
; Każda z nich występuje dokładnie raz. W głównej pętli odweidzamy każdy element, aby upewnić 
; się, że trafimy na każdy z cyklów (i = p[..p[i]]). W pętli wewnętrznej odwracamy cykl, w którym 
; znajduje się liczba i. Wykonujemy p[i] = ~i, co odpowiada operacji p[i] = i dla wejściowej 
; tablicy p. Jeśli liczba p[i] jest nieujemna, to znaczy, że cykl został już odwrócony. 
; W takim przypadku nie odwracamy go ponownie i wracamy do pętli głównej.
; EDI jest indeksem pętli od n-1 do 0.
; EDX przechowuje indeks obecnego elementu w cyklu.
; ECX przechowuje indeks następnego elementu w cyklu.
; EAX przechowuje tymczasowo wartość następnego elementu w cyklu, którą zamierzamy nadpisać.
.inverse:                      
  dec edi
  mov edx, edi                 
  mov ecx, DWORD [rsi + rdi*4] 
.inverse_loop:
  cmp ecx, 0                   
  jge .skip_inverse_loop       ; Cykl został już odwrócony.
  not ecx                      ; Odwracamy cykl.
  mov eax, DWORD [rsi + rcx*4] ; Zapamiętujemy tymczasowo  wartość następnedo element w cyklu.
  mov DWORD [rsi + rcx*4], edx ; Odwracamy element w cyklu.
  mov edx, ecx                 ; Przechodzimy do następnego elementu w cyklu.
  mov ecx, eax                 
  jmp .inverse_loop
.skip_inverse_loop:                     
  cmp edi, 0                 
  ja .inverse

  mov al, 1                    ; Odwracanie powiodło się i zwracamy true.
  ret

; Cofa zmiany w tablicy p. Wejściowa tablica p zawierała liczby niuejemne, niektóre z nich 
; zostały zmienione na ujemne za pomocą operacji not. Na tych zmodyfikowanych
; ponownie wykonujemy not, aby przywrócić ich oryginalną wartość.
; EDI jest indeksem pętli od n-1 do 0. 
.reverse_changes:              
  dec edi
  cmp DWORD [rsi + rdi*4], 0   ; Jeśli p[i] >= 0, to element nie był modyfikowany.
  jge .skip_xor
  not DWORD [rsi + rdi*4]      
.skip_xor:                      
  cmp edi, 0                 
  ja .reverse_changes            

.return_false:
  xor al, al
  ret
