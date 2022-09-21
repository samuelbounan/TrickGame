import numpy as np
import matplotlib.pyplot as plt
L = []
score = 0

with open('res_AI_stefan.txt') as f:
  f.readline()
  for line in f:
    L_split = line.split(" ")
    if len(L_split) >= 2:
      L.append(int(L_split[1]))
  f.close()

for x in L:
  score += x

print("On " + str(len(L)) + " games the total score is " + str(score) + "\n")
