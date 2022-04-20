import sys
import math

n = len(sys.argv)
if n < 3:
  sys.exit("Invalid Input")

k = sys.argv[0]
k_num = k.split(".")
if len(K_num) > 2:
  sys.exit("Invalid Input!")
else:
  for digit in k_num:
    if not digit.isdigit():
      sys.exit("Invalid Input!")
if float(k) < 1 or float(k) != int(k):
  sys.exit("Invalid Input")
else:
  k = int(k)

if n == 3:
  max_iter = 200
  input_filename = sys.argv[1]
  output_filename = sys.argv[2]

else:
  max_iter = sys.argv[1]
  num = max_iter.split(".")
  if len(num) > 2:
    sys.exit("Invalid Input!")
  else:
    for digit in num:
      if not digit.isdigit():
        sys.exit("Invalid Input!")

  if float(max_iter) < 1 or float(max_iter) != int(max_iter):
    sys.exit("Invalid Input!")
  else:
    max_iter = int(max_iter)

  input_filename = sys.argv[2]
  output_filename = sys.argv[3]


datapoints_arr = []

try:
  input1 = open(input_filename)
except IOError:
  sys.exit("An Error Has Occurred")


line1 = input1.readline()

while line1 != "":

  line = line1.split(",")
  line[len(line)-1] = line[len(line)-1].replace("\n", "")

  datapoints_arr.append(line)

  line1 = input1.readline()

input1.close()

centroids_arr = datapoints_arr[:k]

if len(datapoints_arr) < k:
  sys.exit("Invalid Input!")

epsilon = 0.001
Euclidean_norm = math.inf
iter_cnt = 1
point_len = len(datapoints_arr[0])

while (iter_cnt != max_iter) or (float(Euclidean_norm) < epsilon):
  c_size = [0 for i in range(k)]
  c_sum = [[0 for o in range(point_len)] for j in range(k)]

  for point in datapoints_arr:
    min_dis = math.inf
    cluster = 0

    for c in range(k):
      temp_dis = 0

      for l in range(point_len):
        temp_dis += math.pow((float(point[l]) - float(centroids_arr[c][l])), 2)

      if temp_dis < min_dis:
        cluster = c
        min_dis = temp_dis

    for v in range(point_len):
      c_sum[cluster][v] += float(point[v])

    c_size[cluster] += 1

  norm = []

  for i in range(k):
    old_centroid = centroids_arr[i]
    try:
      new_centroid = [c_sum[i][j]/c_size[i] for j in range(point_len)]
    except ZeroDivisionError:
      sys.exit("An Error Has Occurred")
    centroids_arr[i] = new_centroid
    distance = 0

    for l in range(point_len):
      distance += math.pow((float(old_centroid[l]) - float(new_centroid[l])), 2)

    norm.append(distance)

  for i in range(k):
    Euclidean_norm += norm[i]
  Euclidean_norm = math.sqrt(Euclidean_norm)

  iter_cnt += 1

centroids_arr = [[str('%.4f' % (float(centroids_arr[i][j]))) for j in range(point_len)] for i in range(k)]

res = open(output_filename, "w")

for i in range(k):
  final_line = centroids_arr[i][0]
  for j in range(1, point_len):
    final_line += "," + centroids_arr[i][j]

  final_line += "\n"
  res.write(final_line)

res.close()

