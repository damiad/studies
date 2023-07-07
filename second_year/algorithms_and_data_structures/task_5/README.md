# Task: KIN
K-Inversions

Lab for Algorithms and Data Structures, Lab 4. Memory Limit: 64 MB. November 20, 2022, 23:59:59.

## Description

Let a1, . . . , an be a permutation of numbers from 1 to n. A k-inversion in this permutation is a sequence of indices i1, i2, . . . , ik such that 1 ≤ i1 < i2 < . . . < ik ≤ n and ai1 > ai2 > . . . > aik. Your task is to determine the number of k-inversions in the given permutation.

## Input

The input consists of two lines. The first line contains two integers n and k (1 ≤ n ≤ 20,000, 2 ≤ k ≤ 10), representing the number of elements in the permutation and the value of k, respectively. The second line contains the permutation of numbers {1, . . . , n}.

## Output

Your program should print the remainder of the division by 10^9 of the number of k-inversions in the given permutation.

## Example

For the input:

```
4 3
4 3 1 2
```

the correct output is:

```
2
```

---
