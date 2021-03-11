#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

sem_t *s;
int threadCount = 0;
int *in;
int maxPhase, phase = 1;
int n = 1;

int getIndex(int row, int col, int n)
{
    return row * n + col;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleCol(int colNum)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (in[getIndex(j, colNum, n)] > in[getIndex(j + 1, colNum, n)])
            {
                swap(&in[getIndex(j, colNum, n)], &in[getIndex(j + 1, colNum, n)]);
            }
        }
    }
}

void bubbleRowForward(int rowNum)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (in[getIndex(rowNum, j, n)] > in[getIndex(rowNum, j + 1, n)])
            {
                swap(&in[getIndex(rowNum, j, n)], &in[getIndex(rowNum, j + 1, n)]);
            }
        }
    }
}

void bubbleRowReverse(int rowNum)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (in[getIndex(rowNum, j, n)] < in[getIndex(rowNum, j + 1, n)])
            {
                swap(&in[getIndex(rowNum, j, n)], &in[getIndex(rowNum, j + 1, n)]);
            }
        }
    }
}
void *thread(void *i)
{
    int id = *(int *)i;
    while (phase < maxPhase + 1)
    {
        if (threadCount < n)
        {
            sem_wait(&s[id]);
            printf("\nSort %d", id);
            if (phase % 2)
            {
                //Row sort
                if (id % 2)
                {
                    //Backward sort
                    bubbleRowReverse(id);
                    printf("brr");
                }
                else
                {

                    //Forward sort
                    bubbleRowForward(id);
                    printf("brf");
                }
            }
            else
            {
                //Column sort
                bubbleCol(id);
                printf("bc");
            }
            threadCount++;
            printf(" Thread %d", threadCount);
        }
        else
        {
            printf("Release");
            threadCount = 0;
            phase++;
            for (int i = 0; i < n; i++)
            {
                sem_post(&s[i]);
            }
        }
    }
    //pthread_exit(NULL);
    printf("\nDone %d", id);
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

    s = (sem_t *)malloc(n * sizeof(sem_t));
    for (int i = 0; i < n; i++)
    {
        sem_init(&s[i], 0, 1);
    }

    maxPhase = ceil(log(n * n) / log(2) + 1);

    pthread_t threads[n];
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);
    for (int i = 0; i < n; i++)
    {
        pthread_create(&threads[i], &attribute, thread, &thread_ids[i]);
    }
    for (int b = 0; b < n; b++)
    {
        pthread_join(threads[b], NULL);
    }

    // pthread_attr_destroy(&attribute);
    // for (int i = 0; i < n; i++)
    // {
    //     sem_destroy(&s[i]);
    // }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", in[getIndex(i, j, n)]);
        }
        printf("\n");
    }

    return 0;
}