# Task: KOM
Strike Committee

Lab for Algorithms and Data Structures, Final Assignment 1. Memory Limit: 128 MB. November 20, 2022, 23:59:59.

## Description

As a result of the decision by the management of Bajtock Factory of Goods to stop heating the factory for cost-saving purposes, a general strike has started. Since the operation of the factory is crucial for the economy of Bajtocja, the management has agreed to mediation. The striking workers are to elect a strike committee from among themselves to represent them in the negotiations.

It is not an easy task. The factory employs n workers with ranks corresponding to their experience. A worker feels represented by a committee member only if their rank differs by at most k. Furthermore, if two committee members have ranks differing by less than l, conflicts arise between them, as the lower-ranked member does not recognize the authority of the more senior one. Help the striking workers elect a committee in such a way that each factory worker feels represented and to avoid internal conflicts among its members.

## Input

The input consists of two lines. The first line contains three integers n, k, and l (1 ≤ n ≤ 500,000, 0 ≤ k, l ≤ 1,000,000,000) representing the number of factory workers and the parameters that the committee must meet according to the requirements of the strikers. The second line contains n integers in the range [0, 1,000,000,000], representing the ranks of the individual workers.

## Output

Your program should print two integers separated by a space: the first one is the size of the minimum committee that meets the requirements of the strikers, and the second one is the number of ways to form such a minimum committee (since this value can be large, we are interested in the result modulo 1,000,000,007).

You can assume that in the given test data, it is always possible to select at least one committee that meets the given conditions.

## Example

For the input:

```
6 2 3
1 2 3 4 5 6
```

the correct output is:

```
2 6
```

---
