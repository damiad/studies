# Task: PRJ
Projekty (Projects)

Lab for Algorithms and Data Structures, Lab 7. Memory Limit: 240 MB. January 1, 2023, 23:59:59.

## Description

Bajtazar has just been promoted to the position of the head of the IT department in a Very Important State Institution. One of his responsibilities is managing IT projects. The institution has prepared a list of potential projects that need to be executed. However, the execution of some projects depends on the successful completion of others. Additionally, each project requires a minimum number of programmers necessary for its execution.

Due to budget cuts, it's not possible to execute all projects. The management has decided that only k projects will be executed. Bajtazar has been given the task of hiring the minimum number of programmers required to execute at least k projects (projects can be executed sequentially, with programmers being transferred from one project to another).

Write a program that will help Bajtazar determine the minimum number of programmers that need to be hired.

## Input

The input consists of the following lines:

- The first line contains three integers: n, m, and k (1 ≤ n ≤ 100,000, 0 ≤ m ≤ 500,000, 0 ≤ k ≤ n), separated by single spaces. They represent the number of projects, the number of dependencies between projects, and the minimum number of projects that need to be executed, respectively.
- The next n lines contain information about the number of programmers required to execute each project. The (i+1)-th line contains an integer pi (1 ≤ pi ≤ 100,000,000), indicating that pi programmers are required to execute the i-th project.
- The following m lines contain information about the dependencies between projects. Each of these lines contains two integers a and b (1 ≤ a, b ≤ n, a ≠ b), separated by a single space, indicating that the execution of project a depends on the completion of project b.

You can assume that the dependencies between projects do not form cycles.

## Output

The output should consist of a single line containing the minimum number of programmers that need to be hired in order to execute k projects.

## Example

For the input:

```
5 3 3
10
500
150
200
100
1 2
1 3
4 5
```

the correct output is:

```
200
```

---
