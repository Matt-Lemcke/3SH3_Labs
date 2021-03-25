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

    char *region = mmap(
        NULL, buffer.st_size,
        PROT_READ, MAP_FILE | MAP_PRIVATE,
        fd, 0);

    while (1)
    {
        fwrite(region, 1, buffer.st_size, stdout);
        int type, amount;
        printf("\nEnter type of resource and how much is needed (type amount): ");
        scanf("%d %d", &type, &amount);
        printf("\n%d %d", type, amount);

        int n = atoi(region[type * 5 + 2]) - amount;
        if (n >= 0)
        {
            region[type * 5 + 2] = n + '0';
        }
        else
        {
            printf("\nNot enough resources");
        }
        msync(region, buffer.st_size, MS_SYNC);
    }
    int unmap_result = munmap(region, buffer.st_size);
    close(fd);
    return 0;
}