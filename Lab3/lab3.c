#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

sem_t *s;                // Semaphore array
int threadCount = 0;     // Counter for number of threads that finish the sort
int *in;                 // Input matrix array
int maxPhase, phase = 1; // Current phase counter and max phase number for shear sort
int n = 1;               // Size of array

// Determine index of input array based on matrix rows and columns
int getIndex(int row, int col, int n)
{
    return row * n + col;
}

// Swap the two given indexes of the input array
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Bubble sort by column
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

// Bubble sort by row from left to right
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

// Bubble sort by row from right to left
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

// Function to write random numbers to NxN matrix to text file based on n and max value
void randomWriteFile(int n, int maxVal){
    FILE *fptr; 
    char fname[20] = "input.txt";
    time_t t;
    fptr = fopen(fname, "w");
    srand((unsigned) time(&t));
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++)
        {
            int randVal = rand() % (maxVal+1);
            if(j == (n-1))
                fprintf(fptr,"%d", randVal);
            else
                fprintf(fptr,"%d,", randVal);
        }
        if(i != (n-1))
            fprintf(fptr, "\n");
    }
    fclose(fptr);
}

// Exection code of the created threads
void *thread(void *i)
{
    int id = *(int *)i; // Get current thread id

    // Cycle through sorting until the max number of phases is reached
    while (phase < maxPhase + 1)
    {
        // Execute if all threads are not finished
        if (threadCount < n)
        {
            // Wait until all threads have finished
            sem_wait(&s[id]);

            // Determine sorting direction
            if (phase % 2)
            {
                //Row sort
                if (id % 2)
                {
                    //Backward sort
                    bubbleRowReverse(id);
                }
                else
                {

                    //Forward sort
                    bubbleRowForward(id);
                }
            }
            else
            {
                //Column sort
                bubbleCol(id);
            }
            // Thread completed
            threadCount++;
        }
        // Execute if all threads have finished
        else
        {
            // Reset threadcounter and increment phase number
            threadCount = 0;
            phase++;

            //Open all thread semaphores
            for (int i = 0; i < n; i++)
            {
                sem_post(&s[i]);
            }
        }
    }
}

// Main function
int main()
{
    randomWriteFile(6, 100);
    FILE *fptr;                   // Input file instance
    int row = 0, col = 0;         // Current row and column identifier
    char fname[20] = "input.txt"; // Input file name
    char str1;                    // Read in character

    /*-------------- read the file -------------------------------------*/
    // Open the file for reading
    fptr = fopen(fname, "r");
    str1 = fgetc(fptr);

    // Count size of array
    while (str1 != EOF)
    {
        if (str1 == '\n')
        {
            n++;
        }
        str1 = fgetc(fptr);
    }
    fclose(fptr);

    // Allocate memory for the input matrix
    in = (int *)malloc(n * n * sizeof(int));
    fptr = fopen(fname, "r");
    str1 = fgetc(fptr);

    // Read in numbers to input matrix array
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

    // Create thread id array
    int thread_ids[n];

    // Print original input
    printf("\nOriginal Matrix\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", in[getIndex(i, j, n)]);
        }
        printf("\n");

        // Initialize thread id array
        thread_ids[i] = i;
    }
    /*-------------- Done reading from file -------------------------------------*/

    // Allocate memory for semaphores
    s = (sem_t *)malloc(n * sizeof(sem_t));

    // Create semaphore for each thread
    for (int i = 0; i < n; i++)
    {
        sem_init(&s[i], 0, 1);
    }

    // Calculate max number of phases for shear sort
    maxPhase = ceil(log(n * n) / log(2) + 1);

    // Create threads
    pthread_t threads[n];
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);
    for (int i = 0; i < n; i++)
    {
        pthread_create(&threads[i], &attribute, thread, &thread_ids[i]);
    }

    // Terminate threads
    for (int b = 0; b < n; b++)
    {
        pthread_join(threads[b], NULL);
    }

    // Print sorted matrix
    printf("\nSorted Matrix\n");
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