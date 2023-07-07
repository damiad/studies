# Task: MEC
Matches

Lab for Algorithms and Data Structures, Lab 3. Memory Limit: 128 MB. November 13, 2022, 23:59:59.

## Description

In a football training session, there are n players participating (where n is an even number). In each match, all players participate, n/2 in each team. The coach wants to organize the teams in such a way that every pair of players has a chance to play against each other in at least one match (i.e., play on opposite teams at least once).

The coach has already proposed the teams for the next m matches. Help him determine whether he was able to achieve his goal.

## Input

The input consists of two lines. The first line contains two integers n and m (4 ≤ n ≤ 40,000, 1 ≤ m ≤ 50), representing the number of players and the number of scheduled matches, respectively. Players are numbered from 1 to n.

Each of the next m lines contains n distinct integers from 1 to n, describing the teams' composition for each match. The first n/2 numbers in each line represent the player numbers playing in the first team, and the second n/2 numbers represent the player numbers in the second team.

## Output

Your program should print one word, either YES or NO, depending on whether every pair of players plays against each other in at least one match or not.

## Example

For the input:

```
6 3
4 6 1 3 5 2
1 4 5 2 3 6
1 2 6 4 5 3
```

the correct output is:

```
YES
```

For the input:

```
6 3
4 6 1 3 5 2
1 4 5 2 3 6
1 2 3 4 5 6
```

the correct output is:

```
NO
```

Explanation for the example: In the first example, every pair of players plays on opposite teams in one match (e.g., players with numbers 1 and 6), in two matches (e.g., players 1 and 2), or even in all three matches (e.g., players 1 and 3). In the second example, players 2 and 3 always play on the same team.

---
