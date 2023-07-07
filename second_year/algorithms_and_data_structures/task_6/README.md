# Task: MAL
Painting the Highway

Lab for Algorithms and Data Structures, Lab 5. Memory Limit: 128 MB. November 27, 2022, 23:59:59.

## Description

Professor Makary, wanting to help the government of Bajtocja, paints the highway for free. The highway has a length of n kilometers and is divided into one-kilometer sections numbered 1, ..., n. The professor has white paint at his disposal.

Initially, the entire highway is black. At night, if Professor Makary suffers from insomnia, he goes out onto the highway with a bucket of paint and paints a certain section of the highway. Unfortunately, sometimes holes appear in the highway, and then during the day, a roller arrives and lays asphalt. The asphalted section of the road becomes black, of course. The professor would like to have real-time information on how many kilometers of the highway are painted white. Help the professor with this responsible task.

## Input

The input consists of several lines. The first line contains an integer n (1 ≤ n ≤ 1,000,000), indicating the length of the highway. The second line contains an integer m (1 ≤ m ≤ 1,000,000), indicating the total number of nights of painting and days of rolling. Each of the next m lines contains two integers a and b (1 ≤ a ≤ b ≤ n) and a letter c. The numbers a and b represent the ends of the painted section, and the letter c describes the event. The letter "B" means that the professor painted the highway, and the letter "C" means that the roller traveled on it.

## Output

After reading each line, your program should print the number of kilometers painted white on the output.

## Example

For the input:

```
12
4
1 5 C
2 10 B
4 6 B
4 7 C
```

the correct output is:

```
0
9
9
5
```

---
