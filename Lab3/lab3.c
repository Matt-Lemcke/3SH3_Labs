#include <stdio.h>
#include <stdlib.h>

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
    while (str1 != EOF)
    {
        if (str1 == '\n')
        {
            n++;
        }
        // printf("%c", str1);
        str1 = fgetc(fptr);
    }
    fclose(fptr);
    in = (int *)malloc(n * n * sizeof(int));
    fptr = fopen(fname, "r");
    str1 = fgetc(fptr);
    while (str1 != EOF)
    {
        printf('%d', getIndex(row, col, n));
        if (str1 == ',')
        {
            in[0] = atoi(str);
            printf('%s', str);
            i = 0;
            col++;
        }
        else if (str1 == '\n')
        {
            row++;
            col = 0;
            i = 0;
        }
        else
        {
            str[i] = str1;
            i++;
        }
        str1 = fgetc(fptr);
    }
    // for (int i = 0; i < n * n; i++)
    // {
    //     // for (int j = 0; j < n; j++)
    //     // {
    //     printf("%d ", input[i]);
    //     // }
    //     printf("\n");
    // }
    return 0;
}