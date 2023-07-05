# Matrix Operations

This project implements various matrix operations in Java based on the requirements of a given task. The goal is to provide a flexible and efficient solution for working with matrices of real numbers, including sparse matrices.

## Task Description

The task involves creating a set of classes that represent different types of matrices. The supported matrix types include:

1. **Dense Matrix**: A matrix represented by a two-dimensional array of double values.
2. **Zero Matrix**: A matrix filled entirely with zeros.
3. **Constant Matrix**: A matrix filled entirely with a single specified value.
4. **Identity Matrix**: A square matrix with ones on the main diagonal and zeros elsewhere.
5. **Diagonal Matrix**: A square matrix with values only on the main diagonal.
6. **Column Matrix**: A matrix with all values in each row being equal.
7. **Row Matrix**: A matrix with all values in each column being equal.
8. **Sparse Matrix**: A matrix represented using a sparse storage format, such as a list of rows with references to non-zero values.

The task requires implementing common matrix operations, including accessing individual elements, matrix multiplication (and addition/substraction), scalar multiplication, calculation of matrix norms, and converting matrices to different representations (e.g., two-dimensional array or string).

## Implementation Guidelines

The code should adhere to a unified interface for all matrix types and strive to reuse common operations through abstract classes/interfaces. The program should handle error conditions, such as incompatible matrix sizes, by using assertions.

Efficiency is an important aspect of the implementation. Special optimizations can be applied to create result matrices that exploit the properties of specific operations, reducing unnecessary computations and memory usage.

## Testing

The project includes a set of tests implemented with JUnit to ensure the correctness of the matrix operations. You can run the tests by executing the `MatrixTest` class. You are encouraged to add your own tests to validate the functionality of the matrix operations and ensure any modifications or extensions do not introduce regressions.
