# Poisson Differential Equation

To solve the Poisson differential equation for a function \( u : [0, 1]^2 \to \mathbb{R} \) satisfying

\[ -\frac{\partial^2 u(x, y)}{\partial x^2} - \frac{\partial^2 u(x, y)}{\partial y^2} = f(x, y), \]

with \( f \) being a given continuous function on \( [0, 1]^2 \) and \( u \) vanishing on the boundary of \( [0, 1]^2 \), we discretize the problem by considering an approximation \( u_h \) on a grid. Here are the steps:

### Grid Setup:

- Choose a natural number \( N > 1 \) and define points \( (x_i, y_j) \) where \( x_i = \frac{i}{N} \), \( y_j = \frac{j}{N} \) for \( i, j = 0, \ldots, N \).
- Let \( h = \frac{1}{N} \), thus \( x_i = ih \), \( y_j = jh \).

### Discretization:

- Approximate the second derivatives using central differences:
  \[
  \frac{1}{h^2} \left(-u_{i-1,j} - u_{i,j-1} + 4u_{ij} - u_{i+1,j} - u_{i,j+1}\right) = f_{ij},
  \]
  where \( f_{ij} = f(x_i, y_j) \).

### Matrix Formulation:

- This leads to a system of linear equations \( A \mathbf{u} = \mathbf{f} \) with a block tridiagonal matrix \( A \):
  \[
  A = \frac{1}{h^2} \begin{bmatrix}
  T & -I & 0 & \cdots & 0 \\
  -I & T & -I & \cdots & 0 \\
  0 & -I & T & \cdots & 0 \\
  \vdots & \vdots & \vdots & \ddots & \vdots \\
  0 & 0 & 0 & -I & T
  \end{bmatrix},
  \]
  where \( T \) is a tridiagonal matrix:
  \[
  T = \begin{bmatrix}
  4 & -1 & 0 & \cdots & 0 \\
  -1 & 4 & -1 & \cdots & 0 \\
  0 & -1 & 4 & \cdots & 0 \\
  \vdots & \vdots & \vdots & \ddots & \vdots \\
  0 & 0 & 0 & -1 & 4
  \end{bmatrix}.
  \]

### Numerical Solution:

- Use iterative methods such as the Conjugate Gradient method to solve the sparse system efficiently.
- During each iteration, compute the residual norm \( \| \mathbf{b} - A \mathbf{x}_k \| \) to check convergence.

### Implementation:

- Write a script in Octave or MATLAB to implement the above steps, including setting up the matrix \( A \) and solving using an iterative solver.
- For \( f \), you can use simple polynomial functions, e.g., \( f(x, y) = x \).

### Visualization:

- Plot the solution \( u_h \) using functions like `mesh` or `surf`.

By following these steps, you can approximate the solution to the Poisson equation on a discrete grid with high accuracy.
