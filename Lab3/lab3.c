#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <pthread.h> 

int getIndex(int row, int col, int n)
{
    return row * n + col;
}

int main()
{
    FILE *fptr;
    int i = 0, n = 1;
    int row = 0, col = 0;
    int *in;
    char str[100];
    char fname[20] = "input.txt";
    char str1;

    /*-------------- read the file -------------------------------------*/
    fptr = fopen(fname, "r");
    str1 = fgetc(fptr);
    printf("%c", str1);
    while (str1 != EOF)
    {
        if (str1 == '\n')
        {
            n++;
        }
        str1 = fgetc(fptr);
        printf("%c", str1);

    }
    fclose(fptr);
    printf("\n---------- DONE READING -----------\n");
    in = (int *)malloc(n * n * sizeof(int));
    fptr = fopen(fname, "r");
    str1 = fgetc(fptr);
    int val = 0;
    while (str1 != EOF)
    {
        if (str1 == ',')
        {
            in[getIndex(row, col, n)] = val;
            col++;
            val = 0;
        }
        else if (str1 == '\n')
        {
            in[getIndex(row, col, n)] = val/10;
            row++;
            col = 0;
            val = 0;
        }
        else
        {
            val = val*10 + atoi(&str1);
        }
        str1 = fgetc(fptr);
    }
    in[getIndex(row, col, n)] = val;
    row++;
    col = 0;
    val = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", in[getIndex(i,j,n)]);
        }
        printf("\n");
    }
// Done reading from file













    return 0;
}