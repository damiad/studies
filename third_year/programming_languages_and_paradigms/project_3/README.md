# Simple Program Analyzer in Prolog

## Introduction

We analyze simple sequential programs whose code is presented as a Prolog list of instructions. Any number of processes with the same code can run in the system. The task is to write a Prolog program that checks for a safety condition, i.e., whether there exists an interleaving of process executions in which at least two processes enter the critical section simultaneously.

## Specification

### Constants
- Integers.

### Variables
- Simple variables (integer type) and arrays of integers. Each array is one-dimensional, with a size equal to the number of processes in the system. Arrays are indexed from 0. All variables (including array elements) are initialized to zero. All variables are global, i.e., accessible to all processes running in the system.
- Each process also has access to a constant named `pid`, whose value is the process identifier, ranging from 0 to N-1, where N is the total number of processes in the system.

### Expressions (Arithmetic and Logical)
- `wyrArytm ::= wyrProste | wyrProste oper wyrProste`
- `wyrProste ::= liczba | zmienna`
- `zmienna ::= ident | array(ident, wyrArytm)`
- `oper ::= +|-|*|/`
- `wyrLogiczne ::= wyrProste operRel wyrProste`
- `operRel ::= <|=|<>`

Variable names (simple, array) are Prolog constants (not numbers), e.g., `x`, `k`, `chce`, 'A'.

### Instructions
- `assign(zmienna, wyrArytm)`: Assigns the value of the given expression to the specified variable (simple or array element). Proceeds to the next instruction.
- `goto(liczba)`: Unconditional jump to the instruction at the given index.
- `condGoto(wyrLogiczne, liczba)`: Conditional jump (if the logical expression is true) to the instruction at the given index; otherwise, proceeds to the next instruction.
- `sekcja`: Critical section. Proceeds to the next instruction.

Instructions are indexed from 1. Each process starts execution from instruction index 1.

### Assumptions
- All variables take values from a certain limited range (e.g., 0..N, where N is the number of processes), although this range is not explicitly given and need not be checked. Hence, the number of states in our system is always finite.
- The programs execute correctly, i.e., correct array references, correct arithmetic expression evaluations, and correct jumps (no division by zero, no out-of-bounds array access, no jump to non-existent instructions).
- Each process runs in an infinite loop (the last instruction of each program is an unconditional jump).

### Task
Write a Prolog procedure `verify/2` invoked as follows:
```prolog
verify(N, Program),
```
where:
- `N` – the number of processes running in the system (>= 1),
- `Program` – the name of the file containing the program (a constant).

The procedure should:
- Check the correctness of the invocation arguments (e.g., the number of processes, the availability of the program file). However, the correctness of the input data (i.e., the program file itself) can be assumed.
- For valid data, the program should:
  - Indicate whether the safety condition is met.
  - In case of a safety violation, print an example of an incorrect interleaving along with information on which processes are in the critical section (process indices).

### Definition of Helper Predicates
Apart from the main predicate `verify/2`, the program should include definitions of the following helper predicates (minor technical changes, e.g., adding a parameter, are permissible):

1. `initState(+Program, +N, -StanPoczątkowy)`
   - `Program` – term representation of the program.
   - `N` – number of processes in the system.
   - `StanPoczątkowy` – representation of the initial state.

2. `step(+Program, +StanWe, ?PrId, -StanWy)`
   - `Program` – term representation of the program.
   - `StanWe` – system state information (values of all variables and program counters of all processes).
   - `StanWy` – system state information after the current instruction is executed by the process identified by `PrId`.

### Program File Format
Text file format:
```prolog
variables(ListOfSimpleVariableNames).
arrays(ListOfArrayVariableNames).
program(ListOfInstructions).
```
All lists are provided in Prolog notation.

### Example Programs
1. Peterson's Algorithm Implementation in the defined language (on the left, in parentheses, the instruction indices):
    ```prolog
    (1)   assign(array(chce, pid), 1)
    (2)   assign(k, pid)
    (3)   condGoto(array(chce, 1-pid) = 0, 5)
    (4)   condGoto(k = pid, 3)
    (5)   sekcja
    (6)   assign(array(chce, pid), 0)
    (7)   goto(1)
    ```

    Representation of the above program (file `peterson.txt`):
    ```prolog
    variables([k]).
    arrays([chce]).
    program([assign(array(chce, pid), 1),
             assign(k, pid),
             condGoto(array(chce, 1-pid) = 0, 5),
             condGoto(k = pid, 3),
             sekcja,
             assign(array(chce, pid), 0),
             goto(1)]).
    ```

2. A very simple incorrect program (`unsafe.txt`):
    ```prolog
    variables([x]).
    arrays([]).
    program([assign(x, pid), sekcja, goto(1)]).
    ```

### Scoring Guide
- 5 points – (correct, good) definition of the `step/4` predicate.
- 2 points – binary information on whether the system meets the safety condition.
- 3 points – finding and printing an incorrect interleaving.
- -1 point – missing (complete) description of the chosen system state representation.

### Additional Important Notes
1. Programs must work correctly under SICStus Prolog on the `students` computer. Programs not meeting this criterion will not be checked.
2. The program may use standard Prolog predicates used in exercises (e.g., `member/2`, `append/3`) and the `lists` library (list operations). 
   - (Loading the library: the source file should begin with the directive `:- ensure_loaded(library(lists)).`)
   - Do not use predicates `findall/3`, `bagof/3`, `setof/3`. Programs using any of these predicates will be graded on a scale of 0-4 points.
3. Optimization is not required, i.e., you may use only simpler (read: more expensive) data structures, e.g., Prolog lists.
4. Negation, cut, `if-then-else` constructs, the `if/3` predicate, etc., may be used correctly in the program.
5. The program should be formatted for readability, e.g., each line should not exceed 80 characters. Example of Prolog program formatting (QuickSort algorithm):
    ```prolog
    qsort([], []).
    qsort([X | L], S) :-       % non-obstructive comment
       partition(L, X, M, W),  % partition the list into sublists
       qsort(M, SM),           % sort sublists
       qsort(W, SW),
       append(SM, [X|SW], S).  % merge the results
    ```

### Submitting the Solution
The solution should consist of a single file named `<student_identifier>.pl` (e.g., `ab123456.pl`), which should be submitted via Moodle. The first line of the file should contain a comment with the author's name (we do not read anonymous submissions).

### Sample Analysis Results
```prolog
?- verify(2, 'peterson.txt').
Program is correct (safe).

?- verify(2, 'peterson-bad0.txt').
Program is incorrect.
Incorrect interleaving:
   Process 0: 1
   Process 1: 1
   Process 1: 2
   Process 1: 3
   Process 0: 2
   Process 0: 3
   Process 0: 4
Processes in the critical section: 1, 0.

?- verify(2, 'any').
Error: missing file named - any

?- verify(0, 'unsafe.txt').
Error: parameter 0 should be a number > 0
```