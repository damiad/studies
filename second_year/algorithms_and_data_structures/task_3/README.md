# Task: SOR
Cell Sort

Lab for Algorithms and Data Structures, Lab 2. Memory Limit: 64 MB. November 6, 2022, 23:59:59.

## Description

Cell sort is a very interesting algorithm with high time complexity for sorting. This algorithm operates step by step, meaning it performs a certain step (a sequence of operations) on the given sequence until the sequence becomes sorted in non-decreasing order.

The sorting step works as follows: we analyze the sequence from left to right and build the resulting sequence on the side. At the beginning of the step, we add the first element of the current sequence to the resulting sequence. Then, for each subsequent element, we place it at the beginning of the auxiliary sequence if the previous element in the original sequence was greater than it, or at the end if the previous element was smaller. For example, in one step of the algorithm, given the sequence: 5, 6, 2, 1, 4, 3, the following auxiliary sequences are created sequentially:
- 5,
- 5, 6,
- 2, 5, 6,
- 1, 2, 5, 6,
- 1, 2, 5, 6, 4,
- 3, 1, 2, 5, 6, 4,

and the last sequence is the result of that step of the algorithm.

Your task is to "sort" the given sequence, i.e., determine how many different sequences transform into the given sequence in one step of the cell sort algorithm.

## Input

The input consists of two lines. The first line contains a single integer n (1 ≤ n ≤ 1000), representing the length of the sequence. The second line contains the sequence itself, consisting of n pairwise distinct integers from the set {1, . . . , n} that need to be sorted.

## Output

Your program should print the remainder of the division by 10^9 of the number of different sequences that transform into the given sequence in one step of the cell sort algorithm.

## Example

For the input:

```
4
1 2 3 4
```

the correct output is:

```
8
```

For the input:

```
4
4 3 2 1
```

the correct output is:

```
0
```

Explanation for the example: In one step of the sorting algorithm, the following sequences transform into the sequence 1, 2, 3, 4:
- 1, 2, 3, 4,
- 4, 3, 2, 1,
- 2, 1, 3, 4,
- 3, 2, 1, 4,
- 2, 3, 1, 4,
- 2, 3, 4, 1,
- 3, 4, 2, 1,
- 3, 2, 4, 1,

while no other sequence transforms into the sequence 4, 3, 2, 1.

---
