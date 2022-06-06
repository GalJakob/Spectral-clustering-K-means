
import math
import numpy as np
import pandas as pd
import csv
import sys

n = len(sys.argv)

k = sys.argv[1]
try:
    k = float(k)
except ValueError:
    sys.exit("Invalid Input")

if float(k) < 1 or float(k) != int(k):
    sys.exit("Invalid Input")
else:
    k = int(k)

if n == 5:
    max_iter = 300
    eps = sys.argv[2]
    input_filename1 = sys.argv[3]
    input_filename2 = sys.argv[4]

elif n == 6:
    max_iter = sys.argv[2]
    try:
        max_iter = float(max_iter)
    except ValueError:
        sys.exit("Invalid Input!")

    if float(max_iter) < 1 or float(max_iter) != int(max_iter):
        sys.exit("Invalid Input!")
    else:
        max_iter = int(max_iter)
    eps = sys.argv[3]
    input_filename1 = sys.argv[4]
    input_filename2 = sys.argv[5]

else:
    sys.exit("Invalid Input")


try:
    input1 = pd.read_csv(input_filename1, sep = ",", header= None).set_index(0)
    input2 = pd.read_csv(input_filename1, sep = ",", header= None).set_index(0)
    input = pd.merge(input1, input2, left_index= True, right_index=True).sort_values(0)
except IOError:
    sys.exit("An Error Has Occurred")

line1 = input1.readline()

while line1 != "":
    line = line1.split(",")
    line[len(line) - 1] = line[len(line) - 1].replace("\n", "")

    datapoints_arr.append(line)

    line1 = input1.readline()

try:
    input1.close()
except IOError:
    sys.exit("An Error Has Occurred")

centroids_arr = datapoints_arr[:k]

if len(datapoints_arr) < k:
    sys.exit("Invalid Input!")

epsilon = 0.001
Euclidean_norm = False
iter_cnt = 1
point_len = len(datapoints_arr[0])

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
    
