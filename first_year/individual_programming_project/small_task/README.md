## Small Task

### Completion Requirements
- Opened: Monday, March 7, 2022, 00:00
- Due by: Saturday, April 2, 2022, 23:59

### Labyrinth
Write a program that finds a path in a multidimensional labyrinth.

### Definitions
The labyrinth is contained within a non-empty k-dimensional interval (k-dimensional rectangular prism) consisting of k-dimensional unit cubes. Each of these cubes can be filled, creating walls of the labyrinth, or empty, creating space through which movement is possible. The labyrinth can be traversed by moving between empty cubes that are adjacent through a (k-1)-dimensional wall.

The position of each cube (whether filled or empty) is determined by specifying its coordinates, which are positive integers.

A path in the labyrinth is a sequence of movements between empty cubes from the starting position to the ending position. The starting and ending positions are defined by specifying the coordinates of two empty cubes. The length of the path is the number of movements. If the ending position is the same as the starting position, the path has a length of zero.

### Input Data
The program reads data from the standard input. Valid data consists of four lines of text. The first three lines contain k integers in the range from 1 to SIZE_MAX, where k is a positive integer that determines the dimension of the labyrinth. These integers represent:

n1, n2, n3, ..., nk,
x1, x2, x3, ..., xk,
y1, y2, y3, ..., yk,

where the ni values specify the sizes of the labyrinth in each dimension, and the xi and yi values specify the coordinates of the starting and ending positions, respectively.

The fourth line contains an integer describing the positions of the labyrinth walls. In the binary expansion of this number, the sum (z1-1) + (z2-1)n1 + (z3-1)n1n2 + ... + (zk-1)n1n2...nk-1 represents the cube with coordinates (z1, z2, z3, ..., zk). If a bit is set to 0, the cube is empty; if it is set to 1, the cube is filled.

The number in the fourth line can be provided in two ways:

- A hexadecimal number starting with the combination of characters "0x" and using the digits 0, 1, ..., 9, a, ..., f, A, ..., F.
- The character "R" followed by five integers in base 10 from 0 to UINT32_MAX: a, b, m, r, s0. These values are used in the manner described below, with the additional condition that the value of m is not zero.

We calculate the values s1, s2, s3, ..., sr using the formula si = (asi-1 + b) mod m. We calculate the remainders wi = si mod n1n2...nk. The binary representation of the number describing the positions of the labyrinth walls sets bit number j if there exists an index i such that j mod 2^32 = wi.

The numbers may be preceded by leading zeros. Numbers within a single line are separated by any number of whitespace characters. There may or may not be a space between the "R" character and the first number. Any number of whitespace characters may appear at the beginning and end of each line.

### Program Description
The program reads the input data and, if it is valid, calculates the length of the shortest path from the starting position to the ending position. The program prints a single line to standard output (terminated by a newline character, '\n' in C) containing the length of the found path (without leading zeros), or the message "NO WAY" if there.

## Constraints
* The dimensions of the labyrinth (n1, n2, ..., nk) are positive integers.
* The starting and ending positions (x1, x2, ..., xk) and (y1, y2, ..., yk) are valid coordinates within the labyrinth.
* The number describing the positions of the walls is either a hexadecimal number or the "R" notation.
* The maximum size for each dimension of the labyrinth is SIZE_MAX.
* The maximum value for the hexadecimal number or the parameters a, b, m, r, s0 is UINT32_MAX.

## Note
The specific algorithm to find the shortest path in the labyrinth is not provided in the problem statement and is left to be implemented by the programmer.
