import numpy as np
import matplotlib.pyplot as plt
L = []
scores = [0, 0]

k = 0
with open('voting-voting.txt') as f:
    f.readline()
    for line in f:
        L.append(int(line.split(" ")[1]))
        k += 1
        if k >= 196:
            break
    f.close()

for (i, el) in enumerate(L):
    scores[i % 2] += el

print("team 0 makes " +
      str((scores[0]-scores[1])/(len(L)/2)) + " more  points than team 1.")
