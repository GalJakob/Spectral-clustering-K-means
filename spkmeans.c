#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "spkmeans.h"


int main(int argc, char *argv[]) {
    int k;
    char *goal;
    char *fileName;
    if (argc == 4) {
        k = atoi(argv[1]);
        goal = argv[2];
        fileName = argv[3];
        calculateByGoal(k, goal, fileName);
    }
    return 0;
}