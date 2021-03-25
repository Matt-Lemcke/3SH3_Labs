#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    struct stat buffer;

    int fd = open("res.txt", O_RDONLY);
    fstat(fd, &buffer);

    char *region = mmap(NULL, buffer.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    while (1)
    {
        //fwrite(region, 1, buffer.st_size, stdout);
        int type, amount;
        printf("\nEnter type of resource and how much is needed (type amount): ");
        scanf("%d %d", &type, &amount);
        // printf("\n%c", region[type * 5 + 2]);

        printf("%c", *region[type * 5 + 2]);

        char b[2];

        char a = region[type * 5 + 2];
        b[0] = a;
        b[1] = '\0';

        int n = atoi(b);
        if (n >= 0)
        {
            n -= amount;
            char nA[2];
            printf("\n%d", n);
            sprintf(nA, "%d", n);
            printf("\n%c", nA[0]);

            region[type * 5 + 2] = '0';
            // msync(region, buffer.st_size, MS_SYNC);
        }
        else
        {
            printf("\nNot enough resources");
        }
    }
    int unmap_result = munmap(region, buffer.st_size);
    close(fd);
    return 0;
}