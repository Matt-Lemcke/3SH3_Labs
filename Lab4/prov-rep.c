#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main()
{
    key_t Key = 1212;
    int semFlag = IPC_CREAT | 0666;

    int semID = semget(Key, 1, semFlag);

    semctl(semID, 0, SETVAL, 1);

    struct sembuf incrementing;
    incrementing.sem_num = 0;
    incrementing.sem_op = 1;
    incrementing.sem_flg = SEM_UNDO;

    struct sembuf decrementing;
    decrementing.sem_num = 0;
    decrementing.sem_op = -1;
    decrementing.sem_flg = SEM_UNDO;

    pid_t childpid;
    struct stat buffer;

    int fd = open("res.txt", O_RDWR);
    fstat(fd, &buffer);
    char *region;
    region = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // fwrite(region, 1, buffer.st_size, stdout);
    if (region == MAP_FAILED)
    {
        printf("Map Failed");
    }
    close(fd);

    if ((childpid = fork()) == -1)
    {
        perror("fork");
        exit(0);
    }
    if (childpid == 0)
    {
        int pageSize;
        char *pageStatus;

        while (1)
        {
            semop(semID, &decrementing, 1);
            pageSize = getpagesize();
            pageStatus = (char *)calloc(1, (buffer.st_size + pageSize - 1) / pageSize);
            mincore(region, buffer.st_size, pageStatus);
            printf("\n\nPage size: %d\n\nCurrent Reource States: \n", pageSize);
            fwrite(region, 1, buffer.st_size, stdout);
            for (int i = 0; i <= sizeof(pageStatus) / sizeof(pageStatus[0]); i++)
            {
                printf("\nPage status at %d: %d", i, pageStatus[i] & 1);
            }
            printf("\n");
            semop(semID, &incrementing, 1);
            sleep(10);
        }
    }
    else
    {
        while (1)
        {
            int type, amount;
            printf("\nEnter type of resource and how much will be added (type amount): ");
            scanf("%d %d", &type, &amount);
            if (type == -1)
            {
                break;
            }

            char b[2];
            semop(semID, &decrementing, 1);
            char a = *(region + type * 5 + 2);
            b[0] = a;
            b[1] = '\0';

            int n = atoi(b);
            printf("\nResources Available: %d", n);
            if (n + amount < 10)
            {

                n += amount;
                char nA[2];

                sprintf(nA, "%d", n);
                printf("\nResources Remaining: %c", nA[0]);

                *(region + type * 5 + 2) = nA[0];
                msync(region, buffer.st_size, MS_SYNC);
            }
            else
            {
                printf("\nToo many resources");
            }
            semop(semID, &incrementing, 1);
        }
        int unmap_result = munmap(region, buffer.st_size);
        close(fd);
        kill(childpid, SIGTERM);
        return 0;
    }
}