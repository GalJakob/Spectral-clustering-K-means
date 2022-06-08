import numpy as np
import pandas as pd
import sys
import math
import mykmeanssp as km

#checking for invalid inputs
num_args = len(sys.argv)

k = sys.argv[1]

try:
    k = float(k)
except ValueError:
    sys.exit("Invalid Input")

if float(k) < 1 or float(k) != int(k):
    sys.exit("Invalid Input")
else:
    k = int(k)

if num_args == 5:
    max_iter = 300
    eps = sys.argv[2]
    input_filename1 = sys.argv[3]
    input_filename2 = sys.argv[4]

elif num_args == 6:
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


input_filename1 = "input_1_db_1.txt"
input_filename2 = "input_1_db_2.txt"
k = 3

#seed
np.random.seed(0)

#creating needed input from given arguments
input1 = pd.read_csv(input_filename1, sep=",", header=None)
input2 = pd.read_csv(input_filename2, sep=",", header=None)
input_temp = pd.merge(input1, input2, on =0)
input = input_temp.sort_values(0)


indices = input.iloc[:,0]
indices_np = indices.to_numpy()

datapoints = input.drop(columns=[0])
datapoints_np = datapoints.to_numpy()


dict = {}

for i in range(len(indices)):
    dict[indices_np[i]] = datapoints_np[i]


n = len(input)

#another input check
if k > n:
    sys.exit("Invalid Input")

i = 1
dim = len(datapoints_np[0])

clusters = []
clusters_indices = []
M = int(np.random.choice(indices))

clusters_indices.append(M)
clusters.append(dict[M])

#algorithm1
while i < k:
    D = [math.inf for x in range(n)]

    for l in range(n):
        Di = [math.pow(np.linalg.norm(dict[l]-dict[clusters_indices[y]]), 2) for y in range (i)]
        D[l] = min(Di)


    P = np.zeros(n)
    for l in range(n):
        P[l] = D[l] / sum(D)

    i += 1
    M = int(np.random.choice(indices, 1, p=P))
    clusters_indices.append(M)
    clusters.append(dict[M])

#needed result
for_km = km.fit(datapoints_np, clusters, k, max_iter, n, dim,  eps)

res = [str(x) for x in clusters_indices]
print(','.join(res))
