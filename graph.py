import numpy as np
import matplotlib.pyplot as plt
L = [162, 147, 87, 162, 96, 138, 123, 149, 147, 145, 107, 83, 68, 141, 67, 85, 73, 108, 105, 117, 134, 104, 107, 94, 114, 115, 150, 147, 93, 90, 94, 89, 148, 88, 88, 90, 134, 127, 70, 115, 148, 78, 141, 118, 76, 139, 97, 141, 76, 107, 71, 129, 134, 102,
     56, 95, 145, 141, 91, 103, 104, 98, 114, 136, 127, 102, 103, 133, 113, 141, 123, 92, 95, 82, 132, 111, 80, 162, 81, 103, 162, 93, 104, 123, 89, 76, 162, 55, 148, 75, 162, 89, 145, 131, 72, 69, 79, 91, 74, 132, 126, 98, 105, 122, 134, 133, 103, 121, 149]


X = np.arange(0, 162, 10)
Y = np.zeros(len(X))

for el in L:
    Y[el//10] += 1

plt.plot(X, Y)
plt.show()
