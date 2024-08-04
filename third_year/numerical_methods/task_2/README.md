# B-Spline Basis Functions

Given a non-decreasing sequence of knots $u_0, \ldots, u_N$ with $N > 2n$, the normalized B-spline basis functions of degree $n$, denoted $N^n_i$, can be defined recursively using the Mansfield-de Boor-Cox formula:

### Base Case:

$$
N^0_i(x) = \begin{cases} 
1 & \text{if } x \in [u_i, u_{i+1}), \\
0 & \text{otherwise}.
\end{cases}
$$

### Recursive Definition:

$$
N^n_i(x) = \frac{x - u_i}{u_{i+n} - u_i} N^{n-1}_i(x) + \frac{u_{i+n+1} - x}{u_{i+n+1} - u_{i+1}} N^{n-1}_{i+1}(x),
$$

provided $u_i \neq \cdots \neq u_{i+n+1}$. If $u_i = \cdots = u_{i+n+1}$, then $N^n_i$ is zero.

### Properties:

- $N^n_i(x)$ is zero outside the interval $[u_i, u_{i+n+1})$.
- At a knot $u_k$ with multiplicity $r$, the spline has continuous derivatives up to order $n - r$.
- The non-zero B-splines of degree $n$ are linearly independent and their sum at any point in $[u_n, u_{N-n})$ is 1.

### De Boor Algorithm:

To evaluate $s(x) = \sum_{i=0}^{N-n-1} d_i N^n_i(x)$ at $x \in [u_n, u_{N-n})$, use:

```cpp
/* x ∈ [uk, uk+1) ⊂ [un, uN−n) */
b[k] = 1; /* N^0_k = 1 */
for ( j = 1; j ≤ n; j++ ) {
    β = (uk+1 − x)/(uk+1 − uk−j+1); /* β = β^(j)_(k−j+1) */
    b[k−j] = β * b[k−j+1]; /* N^j_(k−j) = βN^(j−1)_(k−j+1) */
    for ( i = k−j+1; i < k; i++ ) {
        α = 1 − β; /* α = α^(j)_i */
        β = (ui+j+1 − x)/(ui+j+1 − ui+1); /* β = β^(j)_(i+1) */
        b[i] = α * b[i] + β * b[i+1]; /* N^j_i = αN^(j−1)_i + βN^(j−1)_(i+1) */
    }
    b[k] *= (1 − β); /* N^j_k = αN^(j−1)_k */
}
```

### Simplified Case:

- If knots are equally spaced, $u_i = a + (i - n)h$ where $h = (b - a)/(N - 2n)$, the algorithm simplifies.
- For $n = 3$, let $\hat{h} = (b - a)/M$ for $M = c(N - 2n)$ with $c$ a small natural number (2 to 10). Given $y_j$ values at points $x_j = a + j\hat{h}$, find the spline $s$ minimizing:
  $$
  E = \sum_{j=0}^{M} (s(x_j) - y_j)^2.
  $$

### Implementation:

- Solve the least squares problem using linear equations involving $a_{ji} = N^3_i(x_j)$.
- Plot the spline $s$ and points $(x_j, y_j)$.

By following these steps, you can construct and evaluate B-spline basis functions efficiently.