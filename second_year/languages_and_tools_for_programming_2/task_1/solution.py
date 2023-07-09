import numpy as np
import matplotlib.pyplot as plt

# Dane obserwacje
data = np.array([[1.0, 1.3, 2.6, 0],
                 [2.2, 1.1, 1.2, 1],
                 [2.0, 2.4, 3.8, 1],
                 [1.5, 3.2, 2.1, 0],
                 [3.2, 1.2, 4.2, 1]])

# Przypadek 1: wejścia to A i B
X1 = data[:, :2]

# Przypadek 2: wejścia to B i C
X2 = data[:, 1:3]

Y = data[:, -1]
w0_values = [x / 10 for x in range(0, 11, 1)]
w1_values = np.linspace(2, 3, 11)

# funkcja błędu średniokwadratowego
def MSE(y, y_pred):
    return np.mean((y - y_pred) ** 2)

# wyliczanie błędów
errors1 = np.zeros((11, 11))
errors2 = np.zeros((11, 11))

for i, w0 in enumerate(w0_values):
    for j, w1 in enumerate(w1_values):
        # obliczenie błędu dla cech A i B
        y_pred = 1 / (1 + np.exp(-(w0 * X1[:, 0] + w1 * X1[:, 1])))
        errors1[i, j] = MSE(Y, y_pred)
        # obliczenie błędu dla cech B i C
        y_pred = 1 / (1 + np.exp(-(w0 * X2[:, 0] + w1 * X2[:, 1])))
        errors2[i, j] = MSE(Y, y_pred)

# transpozycja tablic, aby w0 było w osi X, a w1 w osi Y
errors1=errors1.T
errors2=errors2.T

print('Tablica MSE dla cech A i B: \n', errors1)
print('Tablica MSE dla cech B i C: \n', errors2)

# wykresy
plt.figure(figsize=(12, 6))
plt.subplot(121)
plt.imshow(errors1, cmap='hot', interpolation='nearest', origin='lower')
plt.colorbar()
plt.xticks(np.arange(0, 11), w0_values)
plt.yticks(np.arange(0, 11), w1_values)
plt.xlabel('w0')
plt.ylabel('w1')
plt.title('MSE dla cech A i B')
plt.subplot(122)
plt.imshow(errors2, cmap='hot', interpolation='nearest', origin='lower')
plt.colorbar()
plt.xticks(np.arange(0, 11), w0_values)
plt.yticks(np.arange(0, 11), w1_values)
plt.xlabel('w0')
plt.ylabel('w1')
plt.title('MSE dla cech B i C')
plt.tight_layout()
plt.show()


# Program wyświetli dwa wykresy: pierwszy przedstawia błąd średniokwadratowy dla cech A i B,
# a drugi dla cech B i C. Na każdym z wykresów błąd jest przedstawiony jako mapa cieplna, 
# gdzie jasne kolory oznaczają niskie wartości błędu, a ciemne kolory wysokie wartości błędu. 
# Oś X odpowiada wartości w_0, a oś Y odpowiada wartości w_1.