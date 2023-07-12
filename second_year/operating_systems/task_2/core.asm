section .data
; Pierwsze N elementów używamy do zaznaczenia numeru rdzenia do wymiany,
; Drugie N elementów, służy do wstawiania wartości ze stosu, którymi ma nastąpić wymiana.
; Początkowo każdy element ma wartość równą MAX_SIZE (domyślnie rdzenie czekają na siebie).
sync_array: times 2*N dq -1      ; Ograniczenie na N: N < 2^63 (inaczej na pewno nie starczy pamięci).

section .text 
global core
	
; Funkcje dostarczone z zewnątrz:
extern get_value
extern put_value
	
; Wyrównuje stos do wielokrotności 16 i wywołuje funkcję
; ze swojego argumentu czyli get_value lub put_value.
; Zdejmuje ze stosu wcześniej zabezpieczone wartości rejestrów rsi i rdi.
%macro check_stack_and_call 1
	push rdi
	bt sp, 3                     ; Stos podzielny przez 8, zatem wystarczy sprawdzić przystaje do 8 mod 16.
	jc %%align_stack
	call %1
%%aligned_stack:
	pop rdi
	pop rsi
	%ifidn %1, get_value
	push rax
	%endif
	jmp .main_loop
	
%%align_stack:
	push rax
	call %1
	pop rdx
	jmp %%aligned_stack
	%endmacro
	
	
; Argumenty funkcji core:
; uint64_t core(uint64_t n, char const * p)
; n, w rejestrze rdi - numer rdzenia, 
; p, w rejestrze rsi - wskaźnik na zapis ASCIIZ obliczenia.
; Obliczenie składa się z operacji wykonywanych na stosie, który na początku jest pusty.
; Po zakończeniu obliczeń wynikiem funkcji core, jest wartość z wierzchołka stosu.
core:
	; Aby na koniec przywrócić stos, musimy zapamniętać jego stan z przed wywołania funkcji core.
	push rbx
	mov rbx, rsp
	
; Główna pętla obliczeń, która interpretuje kolejne znaki z polecenia.
.main_loop:
	movzx edx, byte [rsi]        ; Przy okazji wyzerowane zostaną starsze bity rejestru.
	inc rsi
	cmp dl, '-'
	jz .negate
	cmp dl, 'n'
	jz .push_core_number
	cmp dl, 'G'
	jz .call_get_value
	
	pop rax                      ; W każdym z pozostałych przypadków, potrzebujemy wartości z wierzchołka stosu.
	test dl, dl
	jz .return
	cmp dl, '+'
	jz .plus
	cmp dl, '*'
	jz .multiply
	cmp dl, 'B'
	jz .skip_characters
	cmp dl, 'D'
	jz .duplicate_top
	cmp dl, 'C'                  ; Zdejmuje wartość ze stosu i porzuca ją.
	jz .main_loop
	cmp dl, 'E'
	jz .swap
	cmp dl, 'P'
	jz .call_put_value
	cmp dl, 'S'
	jz .synchronize
	
	; Obliczenie poprawne, zatem trafiliśmy na cyfrę od 0 do 9. Niepotrzebnie 
	; zdjęliśmy wartość ze stosu, ale to efektywniejsze niż sprawdzać czy była to cyfra.
	push rax					 
	sub dl, '0'                  ; Zamiana na wartość numeryczną.
	push rdx
	jmp .main_loop
	
; Ładuje wartość z wierzchołka stosu (rax) jako wynik i przywraca stos do stanu sprzed wywołania funkcji core.
.return:
	mov rsp, rbx
	pop rbx
	ret
	
; Zdejmuje dwie wartości ze stosu, oblicza ich sumę i wstawia wynik na stos.
; Pierwsza z nich już w rax.
.plus:
	add [rsp], rax
	jmp .main_loop
	
; Zdejmuje dwie wartości ze stosu, oblicza ich iloczyn i wstawia wynik na stos.
; Pierwsza z nich już w rax.
.multiply:
	pop rdx
	mul rdx
	push rax
	jmp .main_loop
	
; Neguje arytmetycznie wartość na wierchołku stosu.
.negate:
	neg QWORD [rsp]
	jmp .main_loop
	
; Wstawia na stos numer rdzenia (zapisany w rdi).
.push_core_number:
	push rdi
	jmp .main_loop
	
; Zdejmuje wartość ze stosu (rax), jeśli teraz na wierzchołku stosu jest wartość
; różna od zera, przesuwa się o zdjętą wcześniej ilość operacji.
.skip_characters:
	cmp QWORD [rsp], 0
	jz .main_loop
	
	add rsi, rax
	jmp .main_loop
	
; Duplikuje wartość na wierzchu stosu (mamy ją już w rax).
.duplicate_top:
	push rax
	push rax
	jmp .main_loop
	
; Zamienia miejscami dwie wartości na wierzchu stosu. Pierwsza już w rax.
.swap:
	pop rdx
	push rax
	push rdx
	jmp .main_loop
	
; Wstawia na stos wartość uzyskaną z wywołania get_value(uint64_t n).
; Wartość rejestrów rsi i rdi zabezpieczamy na stosie przed nadpisaniem.
.call_get_value:
	push rsi
	check_stack_and_call get_value
	
; Zdejmuje wartość w ze stosu (rax) i wywołuje funkcję void put_value(uint64_t n, uint64_t w);
; Wartość rejestrów rsi i rdi zabezpieczamy na stosie przed nadpisaniem.
.call_put_value:
	push rsi
	mov rsi, rax                  ; W rejestrze rsi musi się znaleźć wartość zdjęta ze stosu.
	check_stack_and_call put_value
	
; Synchronizuje rdzenie, zdejmuje wartość ze stosu (numer rdzenia m) w rax.
; Czeka na operację S rdzenia m ze zdjętym ze stosu numerem rdzenia n.
; Zamienia wartości na wierzchołkach stosów rdzeni m i n.
; W indexie równym numerowi naszego rdzenia zapisywany jest numer rdzenia z którym chemy się wymienić.
; Pod indeksem równym naszemu numerowi rdzenia + N zapisujemy wartość którą chcemy wymienić.
.synchronize:
	lea rdx, [rel sync_array]
	pop QWORD [rdx + 8 * (rdi+N)] ; Wartość do wymiany.
	mov [rdx + 8 * rdi], rax      ; Zaznaczamy na jaki rdzeń czekamy.
.wait_core_ready:             	  ; Czekamy na rdzeń do wymiany.
	cmp rdi, [rdx + 8 * rax]
	jnz .wait_core_ready
	
	push QWORD [rdx + 8 * (rax+N)]
	mov [rdx + 8 * rax], rax      ; Sygnalizujemy zakończenie wymiany.
.wait_core_done:              	  ; Czekamy aż drugi rdzeń również zakończy wymianę.
	cmp rdi, [rdx + 8 * rdi]
	jnz .wait_core_done
	
	jmp .main_loop
