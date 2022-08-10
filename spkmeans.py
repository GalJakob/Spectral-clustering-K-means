import sys
import numpy as np
import spkmeansmodule


def main():
    k = int(sys.argv[1])
    goal = sys.argv[2]
    fileName = sys.argv[3]
    if goal == "spk":
        runSpkMeans(k, fileName)
    else:
        spkmeansmodule.execByGoal(k, goal, fileName) 

if __name__ == '__main__':
    main()

def runSpkMeans(k, fileName):
    """ performs the spectral clustering """
    dataPoints = spkmeansmodule.execSpkAlg(k, fileName)

