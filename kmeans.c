#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
void assignVars(int,int,char*);

int main(int argc, char *argv[])
{

    /* with max_iter */
    char c;
    FILE* filePtr;
    int K = atoi(argv[1]);
    int max_iter = atoi(argv[2]);
    char **fileNamePtr;
    fileNamePtr = &argv[3];

    filePtr = fopen(*fileNamePtr,"r");
    c = fgetc(filePtr);
    printf("asa %c",c);
    K++;
    max_iter++;
    fclose(filePtr);

    /* char** ptr = *argv[3];
    printf("%s",*ptr); */

    printf("number of arguments: %d", argc - 1);
    /*
    printf("k : %s\n", argv[1]);

    printf("max_iter : %s\n", argv[2]);
    printf("file name : %s\n", argv[3]);

    ptr = fopen("test.txt", "r");
 */
    return 0;
}
