/* major functions */
void execByGoal(int k, char *goal, char *filename);
void createWeightedAdjMat(double*** weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords);
void createDiagonalDegreeMat(double *** res, double *** mat, int n);


/* utils functions */
void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg);
void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr);
double getEuclideanNorm(double* point1,double* point2,int numOfCords);
double SumMatRow(double * matRow, int n);
double ** hofchit (double ** ddg, int n);
double** buildRotMatP(double** LnormMat,double numOfPoints);
double getPivotAndHisIdxs(double **mat, double numOfPoints, int *pivRow, int *pivCol);
double arcCot(double x);
double getPhiAngle(double **LnormMat, int pivRow, int pivCol);
double **allocateAndCreateP(double phiAngle, int numOfPoints,int pivRow,int pivCol);
