# Task: CHO
Choinka (Christmas Tree)

Lab for Algorithms and Data Structures, Final Exam 2. Memory Limit: 256 MB. December 18, 2022, 23:59:59.

## Description

It's Christmas time! Bajtazar is starting to plan how to decorate his Christmas tree. He already has a preliminary layout for the placement of ornaments, but he will still make changes by replacing some ornaments with others. His refined aesthetic taste requires him to pay special attention to the branches of the tree where all - or almost all - ornaments have the same color. Write a program that will help him with his planning.

## Input

The input consists of the following lines:

- The first line contains two integers: n (2 ≤ n ≤ 200,000) and q (1 ≤ q ≤ 200,000). The number n represents the number of nodes in the tree representing the Christmas tree, numbered from 1 to n, with node 1 being the root. The number q represents the number of commands to execute.
- The second line contains n - 1 numbers a1, ..., an-1, where 1 ≤ ai ≤ n. The number ai represents the parent node of node i + 1.
- The third line contains n numbers k1, ..., kn, where 1 ≤ ki ≤ 1,000,000,000. The number ki represents the color of the ornament placed in node i.
- Each of the following q lines contains one of two commands:
  - "z v x" (1 ≤ v ≤ n, 1 ≤ x ≤ 1,000,000,000): Change the ornament placed in node v to an ornament with color x.
  - "? v" (1 ≤ v ≤ n): Check if the subtree rooted at node v is almost monochromatic, i.e., if all ornaments in the nodes of this subtree, except for at most one, have the same color.

## Output

For each command of the form "? v" in the input, the output should contain one line with the word "TAK" (YES) if the subtree rooted at node v is almost monochromatic, and "NIE" (NO) otherwise.

## Example

For the input:

```
5 5
1 2 1 3
3 1 2 5 4
? 2
z 5 1
? 2
z 2 2
? 2
```

the correct output is:

```
NIE
TAK
TAK
```

---
