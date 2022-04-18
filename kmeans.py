import sys
import math

"""n = len(sys.argv)
k = sys.argv[0]

filename1 = str(sys.argv[2])

if n == 3:
  max_iter = sys.argv[1]
  filename = sys.argv[2]

elif n == 2:
  filename = sys.argv[1]
  max_iter = math.inf
"""
k=3
max_iter = 1
filename = "input_1.txt"
input1 = open(filename)

datapoints_arr = []
centroids_arr = []
clusters = {}
cnt = 0

line1 = input1.readline()

while line1 != "":

  line = line1.split(",")
  line[len(line)-1] = line[len(line)-1].replace("\n", "")

  datapoints_arr.append(line)

  if cnt < k:
    centroids_arr.append(line)
    clusters[cnt] = []

  cnt +=1
  line1 = input1.readline()

epsilon = 0.001
Euclidean_norm = math.inf
iter_cnt = 0
point_len = len(datapoints_arr[0])

while (iter_cnt != max_iter) or (float(Euclidean_norm) < epsilon):

  for point in datapoints_arr:
    min_dis = math.inf

    for c in range(k):
      temp_dis = 0

      for l in range(point_len):
        temp_dis += math.pow((float(point[l]) - float(centroids_arr[c][l])), 2)

      if temp_dis < min_dis:
        cluster = c
        min_dis = temp_dis

    clusters[cluster].append(point)

  norm = []

  for i in range(k):
    old_centroid = centroids_arr[i]
    now_cluster = clusters[i]
    sum = [0 for i in range(point_len)]

    for point in now_cluster:

      for p in range(point_len):
        sum[p]+= float(point[p])

    sum = [str(float(x)/len(now_cluster)) for x in sum]
    new_centroid = sum
    centroids_arr[i] = new_centroid
    distance = 0

    for l in range(point_len):
      distance += math.pow((float(old_centroid[l]) - float(new_centroid[l])), 2)

    norm.append(distance)

  for i in range(k):
    Euclidean_norm += norm[i]
  Euclidean_norm = math.sqrt(Euclidean_norm)

  iter_cnt += 1

print(centroids_arr)
print(iter_cnt)

