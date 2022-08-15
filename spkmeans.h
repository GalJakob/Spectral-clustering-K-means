/* major functions */
void execByGoal(int k, char *goal, char *filename);
void createWeightedAdjMat(double*** weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords);



/* utils functions */
void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg);
void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr);
double getEuclideanNorm(double* point1,double* point2,int numOfCords);


