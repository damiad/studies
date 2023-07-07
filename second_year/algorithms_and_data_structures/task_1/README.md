# Task: BAZ
Bazarek

Lab for Algorithms and Data Structures, Lab 1 (Warm-up). Memory Limit: 128 MB. November 2, 2022, 23:59:59.

## Description

Little Bajtek is spending his vacation at Grandma Bajtula's place. Every morning, Grandma goes to the marketplace to buy certain products. The boy quickly noticed an interesting pattern: every day, Grandma spends an amount on shopping that is expressed as an odd integer. Bajtek soon realized that this pattern is a characteristic feature of all Bajtula's grandmas.

Every day, Grandma Bajtula buys at most one copy of each of the n products available at the marketplace. In her caution, Grandma doesn't want to take too much money with her for shopping. One day, she asked Bajtek for guidance on how much money she should bring if she wants to buy exactly k products at the marketplace. Unfortunately, Bajtek doesn't know which products Grandma intends to buy, so the amount she takes must be sufficient for any k products (so that the sum of their costs is odd). The same situation repeats several times. Bajtek decided to approach the matter methodically and write a program that, given the prices of all the products available at the marketplace, would answer Grandma's questions.

## Input

The first line of the input contains a single integer n (1 ≤ n ≤ 1,000,000), indicating the number of products available at the marketplace. The second line contains n integers in the range [1, 10^9], representing the prices of individual products. These numbers are given in non-decreasing order.

The third line contains a single integer m (1 ≤ m ≤ 1,000,000), indicating the number of days Bajtek will spend at Grandma's. Each of the next m lines contains a single integer ki (1 ≤ ki ≤ n), indicating the number of products Grandma intends to buy on that day.

## Output

Your program should print m lines of output. In the i-th line (for i = 1, ..., m), there should be a single integer representing the maximum odd price of ki products. If it is not possible to select ki products whose total price would be odd, the i-th line of the output should contain the number -1.

## Example

For the input:

```
4
1 2 3 4
3
2
3
4
```

the correct output is:

```
7
9
-1
```

## Constraints

- 1 ≤ n ≤ 1,000,000 (number of products)
- 1 ≤ ki ≤ n (number of products Grandma intends to buy on a specific day)
- 1 ≤ m ≤ 1,000,000 (number of days Bajtek spends at Grandma's)
- 1 ≤ prices of products ≤ 10^9

---
