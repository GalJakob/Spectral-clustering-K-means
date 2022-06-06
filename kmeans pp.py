
import math
import numpy as np
import pandas as pd

n = 100
i = 1
k = 15
np.random.seed(0)
size = 3

datapoints = np.zeros((n, size))
clusters = np.zeros((k, size))
M = np.random.choice(datapoints)
I = [x for x in range(n)]

while i < k:
    D = [math.inf for y in range(n)]

    for o in range(n):
        temp = math.inf

        for j in range(i):

            for s in range(size):
                temp += math.pow((float(datapoints[o, s]) - M[s]), 2)

        if D[o] > temp:
            D[o] = temp

    for L in range(n):
        P = [0.0 for x in range(n)]
        P[L] = D[L] / sum(D)

    i += 1
    M = datapoints[int(np.random.choice(I, 1, p=P))]
    
