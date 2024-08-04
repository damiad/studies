# Parametric Curves

A parametric curve is a set of vectors (or points) $S = \{ s(t) : t \in [a, b] \}$ defined by a vector function $s$, called the parametrization. The length of the curve can be calculated using the formula:

$$
\ell(S) = \int_a^b \| s'(t) \|^2 \, dt.
$$

For a cubic B-spline curve parametrized as:

$$
s(t) = \sum_{i=0}^{N-n-1} d_i N^n_i(t), \quad t \in [a, b),
$$

where $N^n_i$ are B-spline basis functions of degree $n$ and $d_i$ are control points, the derivative $s'(t)$ is a quadratic B-spline function. Therefore, the integral involves the square root of a quartic spline function.

### Derivative of the Spline:

$$
s'(t) = \sum_{i=0}^{N-n-1} d_i \left( N^n_i(t) \right)', \quad t \in [a, b),
$$

where

$$
\frac{d}{dt} N^n_i(t) = \frac{n}{u_{i+n} - u_i} N^{n-1}_i(t) - \frac{n}{u_{i+n+1} - u_{i+1}} N^{n-1}_{i+1}(t).
$$

Alternatively,

$$
s'(t) = \sum_{i=0}^{N-n-2} \frac{n}{u_{i+n+1} - u_{i+1}} (d_{i+1} - d_i) N^{n-1}_i(t).
$$

### Numerical Integration:

- Use composite quadrature rules, such as the Composite Simpson's or Composite Gauss-Legendre rules, to numerically evaluate the integral.
- For given nodes and control points, compute the curve length using the chosen quadrature method.

### Implementation:

- Write a script that computes the length of the cubic B-spline curve based on given nodes and control points.
- Assume equally spaced nodes $u_i = i$ for simplicity.
- Use a composite quadrature with intervals $$[u_n, u_{n+1}), \ldots, [u_{N-n-1}, u_{N-n})$$.

### Optimization:

- To minimize computational cost, choose the interval length $h$ such that the error is within a specified tolerance $\epsilon$.
- Compare results with intervals of length $h$ and $h/2$ to ensure accuracy.

By implementing these steps, you can calculate the length of parametric B-spline curves accurately and efficiently.
