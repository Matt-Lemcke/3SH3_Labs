#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argv, char *argc[]){
    if(argv == 2){
        
        struct stat buffer;
        if (stat(argc[1], &buffer) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
    // printf("File type:                ");

    // switch (buffer.st_mode & S_IFMT) {
    //     case S_IFBLK:  printf("block device\n");            break;
    //     case S_IFCHR:  printf("character device\n");        break;
    //     case S_IFDIR:  printf("directory\n");               break;
    //     case S_IFIFO:  printf("FIFO/pipe\n");               break;
    //     case S_IFLNK:  printf("symlink\n");                 break;
    //     case S_IFREG:  printf("regular file\n");            break;
    //     case S_IFSOCK: printf("socket\n");                  break;
    //     default:       printf("unknown?\n");                break;
    // }
    int statchmod = buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    printf("Mode:                     %o\n", statchmod);
    printf("Link count:               %ld\n", (long) buffer.st_nlink);


    struct passwd *name;
    name = getpwuid(buffer.st_uid);
    printf("Owners Name:              %s\n", name->pw_name);
    
    struct group *grp;
    grp = getgrgid(buffer.st_gid);
    printf("Group Name:               %s\n", grp->gr_name);
    // printf("I-node number:            %ld\n", (long) buffer.st_ino);
    // printf("Ownership:                UID=%ld   GID=%ld\n",
    //         (long) buffer.st_uid, (long) buffer.st_gid);

    // printf("Preferred I/O block size: %ld bytes\n",
    //         (long) buffer.st_blksize);
    printf("File size:                %lld bytes\n",
            (long long) buffer.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) buffer.st_blocks);
    
    printf("Last file modification:   %s", ctime(&buffer.st_mtime));
    
    // printf("Last status change:       %s", ctime(&buffer.st_ctime));
    // printf("Last file access:         %s", ctime(&buffer.st_atime));

   exit(EXIT_SUCCESS);
    }else{
        printf("Please enter either a file or directory as the parameter\n");
    }
}