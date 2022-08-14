#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


int main(void)
{
    return 0;
}
double SumMatRow(double * matRow, int n) {
    double res;
    res = 0;
    int i;
    for (i = 0; i < n; i++){
        res += matRow[i];
    }

    return res;
}

double ** TheDiagonalDegreeMat(double ** mat, int n)
{
    int i;
    /*create zero mat*/
    double ** ddg;
    ddg = (double **) calloc(n, sizeof(double *));
    assert(ddg != NULL);
    for (i = 0; i < n; i++){
        ddg[i] = (double *) calloc(n, sizeof(double));
        assert(ddg[i] != NULL);
        ddg[i][i] = SumMatRow(mat[i], n);
    }
    return ddg;
}

double ** hofchit (double ** ddg, int n){
    int i;
    for (i = 0; i < n; i++){
        ddg[i] = (double *) calloc(n, sizeof(double));
        assert(ddg[i] != NULL);
        ddg[i][i] = 1 / sqrt(ddg[i][i]);
    }
    return ddg;
}
