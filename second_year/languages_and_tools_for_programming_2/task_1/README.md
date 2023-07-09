# Task 1: Mean Squared Error Calculation

The task involves calculating the mean squared error (MSE) for a predictive model based on the provided observations. The observations come from two groups (classes) labeled with elements from the set ![equation](https://latex.codecogs.com/svg.latex?%5Cmathcal%7BG%7D%20%3D%20%5C%7B0%2C1%5C%7D).

The observations are as follows:

|  A  |  B  |  C  || Y |
|-----|-----|-----||---|
| 1.0 | 1.3 | 2.6 || 0 |
| 2.2 | 1.1 | 1.2 || 1 |
| 2.0 | 2.4 | 3.8 || 1 |
| 1.5 | 3.2 | 2.1 || 0 |
| 3.2 | 1.2 | 4.2 || 1 |

Here, A, B, and C are variables (also referred to as features or attributes), and Y represents the label of the group to which each observation belongs.

Let's consider the following predictive model:

![equation](https://latex.codecogs.com/svg.latex?M_%5Cmathbf%7Bw%7D%28x_0%2Cx_1%29%20%3D%20%5Cfrac%7B1%7D%7B1%2Be%5E%7B-%28w_0x_0%2Bw_1x_1%29%7D%7D)

where ![equation](https://latex.codecogs.com/svg.latex?%5Cmathbf%7Bw%7D%20%3D%20%28w_0%2C%20w_1%29) are the weights assigned to the inputs ![equation](https://latex.codecogs.com/svg.latex?x_0) and ![equation](https://latex.codecogs.com/svg.latex?x_1) in two cases:
* inputs ![equation](https://latex.codecogs.com/svg.latex?x_0) and ![equation](https://latex.codecogs.com/svg.latex?x_1) correspond to features A and B, respectively.
* inputs ![equation](https://latex.codecogs.com/svg.latex?x_0) and ![equation](https://latex.codecogs.com/svg.latex?x_1) correspond to features B and C, respectively.

Please write a Python program that calculates the mean squared error of the above model in both cases using the given observations. The program should calculate the errors for all combinations of weight values, where ![equation](https://latex.codecogs.com/svg.latex?w_0%20%5Cin%20%5B0%2C%200.1%2C%200.2%2C%20...%2C%201%5D) and ![equation](https://latex.codecogs.com/svg.latex?w_1%20%5Cin%20%5B2%2C%202.1%2C%202.2%2C%20...%2C%203%5D). For each case, the program should output a 2D array with error values for each combination of weights and a plot of the error as a function of the weights ![equation](https://latex.codecogs.com/svg.latex?w_0) and ![equation](https://latex.codecogs.com/svg.latex?w_1).

The mean squared error is defined by the following formula:

![equation](https://latex.codecogs.com/svg.latex?MSE%20%3D%20%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%28y_i%20-%20%5Chat%20y_i%29%5E2)

where ![equation](https://latex.codecogs.com/svg.latex?y_i) represents the actual values from the data, and ![equation](https://latex.codecogs.com/svg.latex?%5Chat%20y_i) represents the values predicted by the model.