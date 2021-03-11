#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

sem_t s;
int threadCount = 0;
int *in;
int maxPhase;
int n = 1;

int getIndex(int row, int col, int n)
{
    return row * n + col;
}

void *sort(void *i)
{
    int id = *(int *)i;
    for (int phase = 1; phase <= maxPhase; phase++)
    {
    }
    // sem_wait(&s);
    printf("\nsort %d", id);
    threadCount++;
}

int main()
{
    FILE *fptr;
    int row = 0, col = 0;
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
            in[getIndex(row, col, n)] = val / 10;
            row++;
            col = 0;
            val = 0;
        }
        else
        {
            val = val * 10 + atoi(&str1);
        }
        str1 = fgetc(fptr);
    }
    in[getIndex(row, col, n)] = val;
    row++;
    col = 0;
    val = 0;
    int thread_ids[n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", in[getIndex(i, j, n)]);
        }
        printf("\n");
        thread_ids[i] = i;
    }
    /*-------------- Done reading from file -------------------------------------*/
    sem_init(&s, 0, n);

    maxPhase = ceil(2 * log(n) / log(2) + 1);

    pthread_t threads[n];
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);
    for (int i = 0; i < n; i++)
    {
        pthread_create(&threads[i], &attribute, sort, &thread_ids[i]);
    }
    for (int b = 0; b < n; b++)
    {
        pthread_join(threads[b], NULL);
    }

    return 0;
}