# Task 1

## Requirements
Open: Tuesday, March 7, 2023, 00:00
Due: Saturday, April 1, 2023, 23:59

## Permutation Inversion

Implement a function called `inverse_permutation` in assembly language, callable from the C language.

```c
bool inverse_permutation(size_t n, int *p);
```

The function takes a non-empty array of integers `p` as a parameter, along with the size of the array `n`. If the array pointed to by `p` contains a permutation of numbers from 0 to n-1, the function will invert that permutation in-place and return true. Otherwise, it will return false, and the content of the array pointed to by `p` remains unchanged. The function should detect clearly incorrect values of n - see the usage example. However, it is assumed that the pointer `p` is valid.

## Compilation
The solution will be compiled using the following command:

```bash
nasm -f elf64 -w+all -w+error -o inverse_permutation.o inverse_permutation.asm
```