# Task: PAR
Park Bitowy (Bit Park)

Lab for Algorithms and Data Structures, Lab 9. Memory Limit: 240 MB. January 15, 2023, 23:59:59.

## Description

The Bit Park consists of n clearings numbered from 1 to n. Some pairs of clearings are connected by (bidirectional) paths. As befits the Bit Park, the arrangement of paths forms a binary tree with clearing number 1 as its root.

Bajtek and Bajtyna came to the park to play after school. The children decided to play the following game. Alternately, one of the children indicates a clearing number a and a non-negative integer d, and the task of the other child is to find a clearing in the park that is at a distance d from clearing a. If such a clearing does not exist, the child must indicate that.

Bajtek would like to make the game easier for himself. He asked you to write a program that will find the clearings specified by Bajtyna.

## Input

The input consists of the following lines:

- The first line contains a single integer n (2 ≤ n ≤ 500,000), representing the number of clearings in the Bit Park.
- Each of the next n lines contains two integers ai and bi (ai, bi ∈ {−1, 1, 2, . . . , n}), indicating that there are paths from clearing number i to clearings ai and bi. The value −1 indicates that the path does not exist. The input data contains all the edges necessary to uniquely build a rooted tree in clearing number 1.
- The next line contains a single integer m (1 ≤ m ≤ 500,000), representing the number of commands Bajtek received from Bajtyna.
- Each of the next m lines contains two integers a and d (1 ≤ a ≤ n, 0 ≤ d < n).

## Output

Your program should print the numbers of the clearings that are the answers to Bajtyna's questions. If there is more than one clearing as the answer to a given question, your program can print any of them. If the clearing indicated by Bajtyna does not exist, you should print -1 on the corresponding line.

## Example

For the input:

```
8
3 4
-1 6
2 5
-1 -1
7 -1
-1 -1
8 -1
-1 -1
6
1 3
1 4
1 5
6 1
6 4
6 5
```

the correct output is:

```
6
8
-1
2
4
8
```

---
