import sys
import numpy as np
import spkmeansmodule


def main():
    k = int(sys.argv[1])
    goal = sys.argv[2]
    fileName = sys.argv[3]
    spkmeansmodule.execByGoalFromPy(k, goal, fileName)


if __name__ == '__main__':
    main()
