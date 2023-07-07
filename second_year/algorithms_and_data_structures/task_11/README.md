# Task: LEX
Porównywanie leksykograficzne (Lexicographic Comparison)

Lab for Algorithms and Data Structures, Lab 10. Memory Limit: 64 MB. January 22, 2023, 23:59:59.

## Description

Let s = s1s2 . . . sn be an n-letter word composed of lowercase letters of the English alphabet. We will deal with the substrings of this word, which are contiguous fragments of the form s[i. .j] = sisi+1 . . . sj. Our goal is to lexicographically compare different pairs of such substrings.

We say that a word u is lexicographically smaller (i.e., comes before) a word v if:
- Word u is a proper prefix of word v, meaning u is an initial fragment of v that is shorter than v, or
- Words u and v differ at some position, and at the first differing position, u contains a letter smaller than the corresponding letter in word v.

We write this relation as u < v.

For example, the word abaab is (lexicographically) smaller than abaababa, the word abaa is smaller than ababa, but neither abab is smaller than abaab, nor is the word abaab smaller than abaab (i.e., itself).

## Input

The input consists of the following lines:

- The first line contains two integers n and m (1 ≤ n, m ≤ 300,000), representing the length of the word s and the number of queries.
- The second line contains an n-letter word.
- Each of the next m lines contains four integers a, b, c, d (1 ≤ a ≤ b ≤ n, 1 ≤ c ≤ d ≤ n), representing a lexicographic comparison query between the words s[a. .b] and s[c. .d].

## Output

Your program should print m lines to the standard output, each containing a single character: '<', '>', or '=', depending on whether the first substring of the given query is lexicographically smaller, larger, or equal to the second substring.

## Example

For the input:

```
13 3
abaababaabaab
8 13 7 7
6 11 4 6
3 5 11 13
```

the correct output is:

```
<
>
=
```

---
