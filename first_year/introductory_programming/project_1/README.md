## Polynomials (description, solution, and quality assessment)

### Requirements for completion
Opened: Monday, November 8, 2021, 08:30
Due by: Tuesday, November 23, 2021, 20:00

### Introduction
A monomial of the variable x is an expression of the form ax^n, where a is the coefficient and n is a non-negative integer called the degree of the monomial. A monomial with a coefficient of 0 is called the zero monomial.

A polynomial of the variable x is defined as the sum of monomials of the variable x. A polynomial with all zero monomials is called the zero polynomial.

The degree of a non-zero polynomial is the maximum degree among its non-zero monomials. We assume that the degree of the zero polynomial is -1.

Polynomials can be operated on using addition and multiplication.

### Task
Implement a calculator that computes the sum and product of polynomials with integer coefficients.

The calculator has a memory, called the accumulator, which stores one polynomial. The initial value of the accumulator is the zero polynomial.

The calculator performs commands to calculate the sum and product of the accumulator value and the polynomial provided as an argument. The command writes the calculated result to the output and stores it in the accumulator.

### Data format
The program's input consists of a sequence of lines containing commands, terminated by a line starting with a period (.). Each command occupies one line.

A command for calculating the sum starts with the + sign, and a command for calculating the product starts with the * sign. The following characters until the end of the line represent the argument of the command.

The syntax for representing a polynomial is described by the following extended Backus-Naur Form (BNF) notation, with the starting symbol `<polynomial>`:

`<polynomial> ::= "0" | [ "-" ] <monomial> { <operation> <monomial> }`  
`<operation> ::= "+" | "-"`  
`<monomial> ::= "1" | <many> | [ <many> ] "x" [ "^" <many> ]`  
`<many> ::= "1" <digit> { <digit> } | <digit from 2 to 9> { <digit> }`  
`<digit> ::= "0" | "1" | <digit from 2 to 9>`  
`<digit from 2 to 9> ::= "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"`  
Curly brackets denote zero or more repetitions, square brackets enclose optional parts, and the vertical bar denotes an alternative. Auxiliary symbols are enclosed in angle brackets, and terminal symbols are enclosed in quotation marks. The terminal symbols correspond to the characters enclosed in quotation marks.

In addition, the monomials of a polynomial are ordered in descending order of degrees.

There may be any number of spaces in a command line. However, a non-empty sequence of spaces does not occur at the beginning of a line or between two digits.

### Result format
For each executed command, the program writes one line to the output with its result. The syntax for representing a polynomial in the output is the same as in the input. There is exactly one space before and after the "+" or "-" sign in the productions of the auxiliary symbol `<operation>`. Apart from that, there are no other spaces in the output.

### Constraints
The input polynomials have at most 100 monomials.
The degree of each monomial does not exceed 100.
The sum and product of the polynomials have at most 100 monomials.
The coefficients of the polynomials are integers from the range -10^9 to 10^9.
