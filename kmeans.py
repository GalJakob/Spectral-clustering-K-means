
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

while epsilon < 1:
  for point in datapoints_arr:
    min_dis = math.inf
    for centroid in centroids_arr:
      temp_dis = math.pow((float(point[0]) - float(centroid[0])), 2) + \
                 math.pow((float(point[1]) - float(centroid[1])), 2) + \
                 math.pow((float(point[2]) - float(centroid[2])), 2)

      if temp_dis < min_dis:
        cluster = centroids_arr.index(centroid)
        min_dis = temp_dis
    clusters[cluster] += point

  for i in range(len(centroids_arr)):
    old_centroid = centroids_arr[i]
    now_cluster = clusters[i]
    sum = 0
    for point in now_cluster:
      sum+= float(point)

    new_centroid = str(sum / (len(now_cluster)))
    centroids_arr[i] = new_centroid

  print(centroids_arr)
  epsilon += 1
