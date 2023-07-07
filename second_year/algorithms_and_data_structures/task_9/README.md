# Task: KAP
Kapitan (Captain)

Lab for Algorithms and Data Structures, Lab 8. Memory Limit: 240 MB. January 8, 2023, 23:59:59.

## Description

Captain Bajtazar is traversing the waters of the Bajtockie Sea with his invaluable first officer, Bajtek. There are n islands in the sea, numbered from 1 to n. The captain's ship is moored near island number 1. As part of their voyage, the captain plans to sail to island number n.

During the journey, the ship always moves in one of the four cardinal directions: north, south, east, or west. At any given moment, either the captain or the first officer is at the helm. Whenever the ship makes a 90° turn, the person at the helm changes.

Along the way, the ship can make stops at other islands. After each stop, the captain can decide whether to take over the helm as the first officer. In other words, on each segment of the route that leads from one island to another, one of the sailors takes the helm while the ship is heading north or south, and the other sailor takes control when the ship is sailing east or west. In particular, if a certain segment of the route runs exactly in one of the four cardinal directions, only one of the sailors is at the helm during that segment.

The captain is now wondering how to plan the route for the next voyage and the division of labor in order to spend as little time as possible at the helm. At the same time, the captain doesn't care about the length of the route. We assume that the ship travels at a constant speed of one unit per hour.

## Input

The input consists of the following lines:

- The first line contains a single integer n (2 ≤ n ≤ 200,000), representing the number of islands in the sea.
- For simplicity, we use a coordinate system aligned with the cardinal directions for the Bajtockie Sea. Each of the next n lines contains the descriptions of the islands: the i-th line contains two integers xi and yi (0 ≤ xi, yi ≤ 1,000,000,000), representing the coordinates of the i-th island in the sea. Each island has different coordinates.

## Output

The output should consist of a single integer, representing the minimum number of hours the captain will have to steer the ship on the route from island number 1 to island number n.

## Example

For the input:

```
5
2 2
1 1
4 5
7 1
6 7
```

the correct output is:

```
2
```

---
