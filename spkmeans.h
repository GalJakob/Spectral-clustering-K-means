#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

/* typedef  */
typedef struct NODE_P nodeP;
struct NODE_P
{
    double ***pointerToRotMatP;
    nodeP *next;
};
#endif

/* major functions */
void execByGoal(int k, char *goal, char *filename);
void createWeightedAdjMat(double ***weightedAdjMat, double ***pointArrPtr, int *numOfPoints, int *numOfCords);
void createDiagonalDegreeMat(double ***res, double ***mat, int n);
void createTheNormalizedGraphLaplacian(double ***lnorm, double ***wam, double ***ddg, int n);
void performJacobiAlg(double **LnormMat, int numOfPoints);

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
double **transpose(double **mat, int n);
void push(nodeP **head, double **newP);
