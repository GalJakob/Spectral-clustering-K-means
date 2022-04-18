
import math
#n = len(sys.argv)

#k = sys.argv[0]
k = 3
#max_ter = sys.argv[1]
#filename1 = str(sys.argv[2])
filename = "input_1.txt"

# if (n == 3):
#   max_ter = sys.argv[1]
#   filename = sys.argv[2]

#elif ( n == 2):
#    filename = sys.argv[1]

input1 = open(filename)

datapoints_arr = []

centroids_arr = []
cnt = 0

clusters = {}

line1 = input1.readline()
while line1 != "":
  line = line1.split(",")
  line[-1] = line[-1].replace("\n", "")
  datapoints_arr.append(line)
  if cnt < k:
    centroids_arr.append(line)
    clusters[cnt] = []
  cnt +=1
  line1 = input1.readline()


epsilon = 0
Euclidean_norm = 0
max_iter = math.inf
iter_cnt = 0
#iter_cnt < max_iter & Euclidean_norm < epsilon

while epsilon < 101:

  for point in datapoints_arr:
    min_dis = math.inf
    for c in range(k):
      temp_dis = 0
      for l in range(len(point)):
        temp_dis += math.pow((float(point[l]) - float(centroids_arr[c][l])), 2)
      if temp_dis < min_dis:
        cluster = c
        min_dis = temp_dis

    clusters[cluster].append(point)

  norm = []

  for i in range(k):
    old_centroid = centroids_arr[i]
    now_cluster = clusters[i]
    sum = [0 for i in range(len(point))]
    for point in now_cluster:
      for p in range(len(point)):
        sum[p]+= float(point[p])
    sum = [str(float(x)/len(now_cluster)) for x in sum]
    new_centroid = sum
    centroids_arr[i] = new_centroid
    distance = 0
    for l in range(len(point)):
      distance += math.pow((float(old_centroid[l]) - float(new_centroid[l])), 2)
    norm.append(distance)

  for i in range(k):
    Euclidean_norm += norm[i]
  Euclidean_norm = math.sqrt(Euclidean_norm)


  epsilon += 1
print(centroids_arr)


