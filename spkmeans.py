import sys
import numpy as np
import pandas as pd
import spkmeansmodule
import math
np.random.seed(0)


def kMeansPP(dataPoints,n,k):
    """ creates k centroids for kMeans """
    pdPoints = pd.DataFrame(dataPoints)
    sortedByFirstCol = pdPoints.sort_values(by=[0])
    indices = pdPoints.iloc[:, 0]
    indices_np = indices.to_numpy()
    print(indices_np)

    datapoints = sortedByFirstCol.drop(columns=[0])
    print(dataPoints)
    datapoints_np =datapoints.to_numpy()

    dict = {}

    for i in range(len(indices)):
        dict[indices_np[i]] = datapoints_np[i]

    dim = len(datapoints_np[0])

    clusters = []
    clusters_indices = []
    M = int(np.random.choice(indices))

    clusters_indices.append(M)
    clusters.append(list(dict[M]))

    print("cl  \n",clusters)
    print("ind    \n",clusters_indices)
    print(datapoints_np)
    return

    i = 1
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
        clusters.append(list(dict[M]))

    datapoints_np = datapoints.to_numpy().tolist()
    return datapoints_np


def main():
    k = int(sys.argv[1])
    goal = sys.argv[2]
    fileName = sys.argv[3]
    nKMat = spkmeansmodule.execByGoalFromPy(k, goal, fileName)
    pointsArr = np.array(nKMat)
    clusters = kMeansPP(pointsArr,len(pointsArr),k)
    print(clusters)
    # dataPointsArray = np.array(dataPoints)
    # clusters = Kmeansplusplus(dataPointsArray)
    # spkmeansmodule.kmeans(dataPoints, clusters, np.size(dataPointsArray, 1))


if __name__ == '__main__':
    main()
