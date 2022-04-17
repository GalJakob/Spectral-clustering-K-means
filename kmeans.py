import numpy as np
import matplotlib as plt
import pandas as pd

#n = len(sys.argv)

#k = sys.argv[0]
#max_ter = sys.argv[1]
#filename1 = str(sys.argv[2])
filename = "input_1.txt"

# if (n == 3):
#   max_ter = sys.argv[1]
#   filename = sys.argv[2]

#elif ( n == 2):
#    filename = sys.argv[1]

x = "noa"
print(x)

input1 = open("input_1.txt")
w = input1.readline().split(",")
i = 0

while i < len(w):
  print(w[i])
  i+=1
  w = input1.readline()
