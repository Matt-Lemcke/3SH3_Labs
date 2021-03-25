#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main()
{
    // Initialize shared semaphore key
    key_t Key = 1212;
    int semFlag = IPC_CREAT | 0666;

    // Create semaphore initialized at 1
    int semID = semget(Key, 1, semFlag);
    semctl(semID, 0, SETVAL, 1);

    // Define semaphore incrementing characteristics
    struct sembuf incrementing;
    incrementing.sem_num = 0;
    incrementing.sem_op = 1;
    incrementing.sem_flg = SEM_UNDO;

    // Define semaphore decrementing characteristics
    struct sembuf decrementing;
    decrementing.sem_num = 0;
    decrementing.sem_op = -1;
    decrementing.sem_flg = SEM_UNDO;

    // Open text file for read and write
    int fd = open("res.txt", O_RDWR);

    //Initialize file buffer
    struct stat buffer;
    fstat(fd, &buffer);

    // Map text file contents to a memory location pointed to by region
    char *region;
    region = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Check for success or failure of mapping file
    if (region == MAP_FAILED)
    {
        printf("Map Failed");
    }
    close(fd);

    // Process loop
    while (1)
    {
        // Collect user info for subtracting from resource allocation
        int type, amount;
        printf("\nEnter type of resource and how much is needed (type amount): ");
        scanf("%d %d", &type, &amount);

        // End process if termination command given
        if (type == -1)
        {
            break;
        }

        // Semwait to enter critical region
        semop(semID, &decrementing, 1);

        // Read in amount of resource available for the given type
        char a = *(region + type * 5 + 2);
        char b[2];
        b[0] = a;
        b[1] = '\0';
        int n = atoi(b);
        printf("\nResources Available: %d", n);

        // Check if there is enough resources available
        if (n - amount >= 0)
        {
            // Subtract from available resources
            n -= amount;

            // Change available resource value in memory
            char nA[2];
            sprintf(nA, "%d", n);
            printf("\nResources Remaining: %c", nA[0]);
            *(region + type * 5 + 2) = nA[0];

            // Sync changes to file
            msync(region, buffer.st_size, MS_SYNC);
        }
        // Do not remove resources if there is not enough available
        else
        {
            printf("\nNot enough resources");
        }

        // Semsignal to exit critical region
        semop(semID, &incrementing, 1);
    }

    // Unmap file to memory
    int unmap_result = munmap(region, buffer.st_size);
    return 0;
}