import sys
import numpy as np
import pandas as pd
import math
import spkmeansmodule
np.random.seed(0)


def kMeansPP(dataPoints,n,k):
    """ creates k centroids for kMeans algorithm """
    pdPoints = pd.DataFrame(dataPoints)
    pdPoints.insert(0, "0", [i for i in range(n)], True)

    indices = pdPoints.iloc[:, 0]
    indices_np = indices.to_numpy()

    datapoints = pdPoints.drop(columns="0")
    datapoints_np =datapoints.to_numpy()

    dictOfPtsByIdx = {}
    for i in range(len(indices)):
        dictOfPtsByIdx[indices_np[i]] = datapoints_np[i]

    clusters = []
    clusters_indices = []
    M = int(np.random.choice(indices))
    clusters_indices.append(M)
    clusters.append(list(dictOfPtsByIdx[M]))

    i = 1
    while i < k:
        D = [math.inf for x in range(n)]
        for l in range(n):
            Di = [math.pow(np.linalg.norm(dictOfPtsByIdx[l]-dictOfPtsByIdx[clusters_indices[y]]), 2) for y in range (i)]
            D[l] = min(Di)

        P = np.zeros(n)
        for l in range(n):
            P[l] = D[l] / sum(D)

        i += 1
        M = int(np.random.choice(indices, 1, p=P))
        clusters_indices.append(M)
        clusters.append(list(dictOfPtsByIdx[M]))

    datapoints_np = datapoints.to_numpy().tolist()
    return clusters


def main():
    """ this function is called if the spkmeans.py is called via cmd """
    k = int(sys.argv[1])
    goal = sys.argv[2]
    fileName = sys.argv[3]

    nKMat = spkmeansmodule.execByGoalFromPy(k, goal, fileName)
    if(nKMat!=None):
        pointsArrNP = np.array(nKMat)
        clusters = kMeansPP(pointsArrNP,len(pointsArrNP),k)
        datapoints_np = pointsArrNP.tolist()
        centroids = spkmeansmodule.fit(datapoints_np, clusters, k, len(datapoints_np),len(datapoints_np[0]))
        print("huibib",centroids)

if __name__ == '__main__':
    main()
