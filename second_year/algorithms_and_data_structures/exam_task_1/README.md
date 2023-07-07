# Task: GIE
Giełda (Stock Exchange)

ASD, Practical Exam (Easy task). Memory Limit: 128 MB. February 4, 2023, 12:00:00.

## Description

Bajtazar has recently joined the Bajtock Investment Bank, and his salary, to put it mildly, is just not enough for him. Fortunately, the company has a program for exceptional achievements bonuses.

In this program, Bajtazar has no chance of winning, as he has just finished his internship at the company. However, there is another program for outstanding work performance. Here he believes that he has a good chance.

Bajtazar's current task is to calculate various economic indicators, but they all depend to some extent on the most important one: the business cycle indicator. Calculating it is quite difficult, and Bajtazar has made mistakes in the past. In order to have any chance of winning the bonus, Bajtazar came up with the idea of automating the process of calculating the business cycle indicator for any given data. It would improve his work and protect him from many mistakes. Unfortunately, Bajtazar doesn't know how to program (yet!), so he decided to ask for your help.

The indicator is defined as the maximum amount that could have been earned from trading stocks, while observing the following restrictions:
- At any given time, you can own at most one stock.
- Each day, you can perform one of the following actions: buy a stock, sell a stock, or abstain from trading.

Thus, for historical stock prices a1, . . . , an and a sequence of k (k ≥ 0) stock market transactions: (b1, s1), . . . ,(bk, sk) (bi denotes the time of purchase in the i-th transaction, si denotes the time of sale in the i-th transaction), the profit can be calculated using the formula:
P =
Σ
k
i=1
(si − bi)

The sequence of transactions must satisfy the condition:
1 ≤ b1 < s1 < b2 < s2 < . . . < bk < sk ≤ n

## Input

The input consists of the following lines:

- The first line contains a single integer n (1 ≤ n ≤ 500,000), representing the length of the historical stock prices.
- The next line contains n integers in the range [1, 10^9], representing the stock prices on consecutive days (a1, . . . , an).

## Output

Your program should output a single integer, which is the maximum profit Popt, representing the business cycle indicator.

## Example

For the input:

```
7
1 2 3 10 5 6 7
```

the correct output is:

```
11
```

---
