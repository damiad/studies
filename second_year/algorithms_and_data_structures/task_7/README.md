# Task: DYN
Dynamic Sequence

Lab for Algorithms and Data Structures, Lab 6. Memory Limit: 128 MB. December 11, 2022, 23:59:59.

## Description

Implement a data structure that performs basic operations on a sequence of numbers, allowing element insertion. The elements in the sequence are numbered from 0. Initially, the sequence is empty. The following operations are allowed:

- `insert(j, x, k)` - inserts k copies of element x right before position j in the sequence, shifting the elements from position j and onwards to the right by k positions.
- `get(j)` - returns the value of the j-th element in the sequence.

You can assume that for the `insert` operation, 0 ≤ j ≤ n, and for the `get` operation, 0 ≤ j < n, where n is the length of the sequence at the time the operation is executed.

In this task, the input format is disguised to enforce online solutions. In your solution, you can use a ready implementation of balanced binary trees from the internet.

## Input

The input consists of several lines. The first line contains an integer m (1 ≤ m ≤ 500,000), indicating the number of operations. Each of the next m lines contains a lowercase letter indicating the type of operation: "i" for insert and "g" for get. Following the letter, there are numbers separated by spaces: j', x, and k for insert, and j' for get. The parameter j in a given query is determined based on the input number j' using the formula j = (j' + w) mod (n + 1) for the insert operation, and j = (j' + w) mod n for the get operation, where w is the result of the last get operation or 0 if there was no previous get operation, and n is the length of the sequence at the time the operation is executed. The numbers j' and x are non-negative and not greater than 109, and the numbers k are in the range from 1 to 1000.

## Output

For each get operation, your program should print one line containing the result of that operation. You can assume that at least one get operation will always be performed.

## Example

For the input:

```
7
i 0 2 3
i 1 1 2
g 2
i 4 1 1
g 2
i 1 3 2
g 2
```

the correct output is:

```
4
8
8
```

---
