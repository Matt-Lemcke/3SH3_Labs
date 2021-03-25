#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(void)
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
    printf("%c", *(region + 2));

    while (1)
    {
        int type, amount;
        printf("\nEnter type of resource and how much is needed (type amount): ");
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
        if (n - amount >= 0)
        {
            n -= amount;
            char nA[2];

            sprintf(nA, "%d", n);
            printf("\nResources Remaining: %c", nA[0]);

            *(region + type * 5 + 2) = nA[0];
            msync(region, buffer.st_size, MS_SYNC);
        }
        else
        {
            printf("\nNot enough resources");
        }
        semop(semID, &incrementing, 1);
    }
    int unmap_result = munmap(region, buffer.st_size);
    close(fd);
    return 0;
}