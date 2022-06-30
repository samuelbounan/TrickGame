import numpy as np
import matplotlib.pyplot as plt
L = []

with open('result.txt') as f:
    f.readline()
    for line in f:
        L.append(int(line))
    f.close()

ds = 5
X = np.arange(0, 162, ds)
Y = np.zeros(len(X))

for el in L:
    Y[el//ds] += 1

plt.plot(X, Y)
plt.show()
