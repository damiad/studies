# Task: MAR
Marszruta (Itinerary)

ASD, Practical Exam (Easy task). Memory Limit: 128 MB. February 4, 2023, 12:00:00.

## Description

Bajtazar did it! He got the dream bonus! And it's much bigger than he expected. To celebrate, he decided to go on a road trip around Bitocja.

He would like to start the trip in city number 1 and end it in city number n. His goal is to visit each city at least once. Of course, he can only travel on roads (in Bitocja, all roads are bidirectional).

Thanks to the bonus, he doesn't have to look for the shortest route that meets the task requirements. However, his innate frugality (or stinginess, if you prefer) prompts him to look for a route that is no longer than 4n (counting the number of vertices).

You can assume that the roads in Bitocja form a connected graph.

## Input

The input consists of the following lines:

- The first line contains two integers n and m (1 ≤ n ≤ 500,000, 1 ≤ m ≤ 1,000,000), representing the number of cities in Bitocja and the number of roads.
- The next m lines describe the roads in Bitocja, each line containing two integers a and b separated by a single space (1 ≤ a, b ≤ n, a ≠ b), indicating that cities a and b are connected by a road. There may be multiple roads between the same pair of cities.

## Output

In the first line, your program should output a single integer k, indicating the length of the itinerary.
In the second line, you should output a sequence of numbers v1, . . . , vk (v1 = 1, vk = n, {v1, . . . , vk} = {1, . . . , n}) separated by a single space, representing the found route.

## Example

For the input:

```
6 8
1 2
2 3
3 4
4 5
5 6
6 1
1 3
3 5
```

the correct output is:

```
6
1 2 3 4 5 6
```

---
