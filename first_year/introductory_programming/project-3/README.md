# Napier-NAF Arithmetic Operations

## Requirements
Open: Monday, December 6, 2021, 08:30
Due by: Tuesday, January 11, 2022, 20:00

## Introduction
Balanced Binary Representation (BBR) is a positional representation of integers with base 2 and digits -1, 0, 1. The BBR representation of a number is in the form of Non-Adjacent Form (NAF) if the nonzero digits are not adjacent to each other.

Every integer has exactly one NAF representation without leading zeros. For 0, the representation is empty. For a nonzero value x, the least significant digit c is:

- 0 if x is even,
- 1 if x - 1 is divisible by 4,
- -1 if x + 1 is divisible by 4.

The more significant digits of the representation form the representation of the number (x - c) / 2.

The NAF representation minimizes the Hamming weight of a number, which is the number of nonzero digits, in the class of binary representations. For example, Mersenne prime numbers have a weight of 2 in this representation. The NAF representation is used in elliptic curve cryptography, including implementations of the Bitcoin cryptocurrency and the Ethereum platform.

The task is to implement the representation of BBR-NAF that enables efficient arithmetic operations on large integers with a low Hamming weight.

The representation is based on the arithmeticæ localis numeral system described by John Napier in 1617 in his work "Rabdologiæ". It is a binary system, but not positional, rather additive.

In positional systems, the value of a number is determined by the sequence of digits and their respective positions. In additive systems, the value of a number is the sum of its digits.

In arithmeticæ localis, the role of digits is played by letters representing powers of two. In our representation, called Napier-NAF, the digits are integers representing powers of two and their negations. The representation of a digit with a value of 2 ** n is a non-negative integer n. The representation of a digit with a value of -(2 ** n) is a negative integer -n - 1.

The Napier-NAF representation of an integer x is an array of integers, with a length equal to the Hamming weight of the BBR-NAF representation of x. In the array:

- A negative number n indicates the presence of the digit -1 at position -n - 1 in the BBR-NAF representation of x.
- A non-negative number n indicates the presence of the digit 1 at position n in the BBR-NAF representation of x.
- The digit information is ordered from the least significant to the most significant, and the positions are numbered starting from 0.

## Command
The napiernaf.h header file contains the interface for performing arithmetic operations on integers in the Napier-NAF representation:

- The function `void iton(int x, int **a, int *n)` stores the representation of the number x in a dynamically allocated array *a of size *n.
- The function `int ntoi(int *a, int n)` returns the value of the number represented by the n-element array a or 0 if the value exceeds the range of the int type.
- The function `void nadd(int *a, int an, int *b, int bn, int **c, int *cn)` stores the sum of the numbers represented by the an-element array a and the bn-element array b in a dynamically allocated array *c of size *cn.
- The function `void nsub(int *a, int an, int *b, int bn, int **c, int *cn)` stores the difference between the numbers represented by the an-element array a and the bn-element array b in a dynamically allocated array *c of size *cn.
- The function `void nmul(int *a, int an, int *b, int bn, int **c, int *cn)` stores the product of the numbers represented by the an-element array a and the bn-element array b in a dynamically allocated array *c of size *cn.
- The function `void nexp(int *a, int an, int *b, int bn, int **c, int *cn)` stores the result of exponentiation, where the base is represented by the an-element array a and the non-negative exponent is represented by the bn-element array b, in a dynamically allocated array *c of size *cn.
- The function `void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r, int *rn)` divides the number represented by the an-element array a by the nonzero number represented by the bn-element array b. The quotient is stored in a dynamically allocated array *q of size *qn, and the remainder is stored in a dynamically allocated array *r of size *rn.

The function follows the Euclidean division algorithm. The result of integer division of a by a nonzero integer b is an integer quotient q and an integer remainder r that satisfy the conditions:

a = b * q + r,

0 <= r < abs(b),

where abs(b) is the absolute value of b.

Write the napiernaf.c module implementing the napiernaf.h interface.
