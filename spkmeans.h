#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED
/* typedef  */
struct eigensOfMat
{
    double eigenVal;
    double *eigenVec;
};
typedef struct eigensOfMat EIGEN;
/* major functions */
double** execByGoal(int k, char *goal, char *filename);
void createWeightedAdjMat(double ***weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords);
void createDiagonalDegreeMat(double ***res, double ***mat, int n);
void createTheNormalizedGraphLaplacian(double ***lnorm, double ***wam, double ***ddg, int n);
void performJacobiAlg(double **LnormMat, int numOfPoints, int *k, double ***eigenVecsMat, EIGEN **sortedEigensPtr);
void createRenormalizedMat(double ***mat, double ***jacobi, int *k, int n);

/* utils functions */
void assignPoints(double ***pointArrPtr, char **inFileNamePtr, int *numOfPointsArg, int *numOfCordsArg);
void computeNumOfCordsAndPoints(FILE **filePtr, int *numOfCords, int *numOfPoints, char ***inFileNamePtr);
double getEuclideanNorm(double *point1, double *point2, int numOfCords);
double SumMatRow(double *matRow, int n);
double **hofchit(double **ddg, int n);
double **buildRotMatP(double **LnormMat, int numOfPoints);
void getPivotAndHisIdxs(double **mat, int numOfPoints, int *pivRow, int *pivCol);
double arcCot(double x);
double getPhiAngle(double **LnormMat, int pivRow, int pivCol);
double **allocateAndCreateP(double phiAngle, int numOfPoints, int pivRow, int pivCol);
void customFreeForMat(double **mat);
double **multiplyMats(double **mat1, double **mat2, int n);
double **transpose(double **mat, int n, int k);
double normalizedSumRow(int k, double *row);
double **multiplyMatWithVector(double **mat1, double **vec, int n);
int getKeigengapHeuristic(EIGEN *EIGENS, int numOfPoints);
double **createKVecsMat(EIGEN *EIGENS, int numOfPoints, int k);
int getNumOfPoints(char *fileName);
EIGEN *buildEIGENArr(double **productOfPs, double **A, int numOfPoints);

/* print functions */
void printJacobiResults(int numOfPointsArg, int k, double **eigenVectorsMat, EIGEN *sortedEigensPtr);
void printMat(double **mat, int numOfRows,int numOfCols);

/* quicksort */
void swap(EIGEN *a, EIGEN *b);
int partition(EIGEN *arr, int low, int high);
void quickSortByEigenVal(EIGEN *arr, int low, int high);
#endif
