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

    int statchmod = buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    printf("Mode:                     %o\n", statchmod);
    printf("Link count:               %ld\n", (long) buffer.st_nlink);
    
    struct passwd *name;
    name = getpwuid(buffer.st_uid);
    printf("Owners Name:              %s\n", name->pw_name);
    
    struct group *grp;
    grp = getgrgid(buffer.st_gid);
    printf("Group Name:               %s\n", grp->gr_name);
    printf("File size:                %lld bytes\n", (long long) buffer.st_size);
    printf("Blocks allocated:         %lld\n", (long long) buffer.st_blocks);
    printf("Last file modification:   %s", ctime(&buffer.st_mtime));
    printf("Name:                     %s\n", argc[1]);

    exit(EXIT_SUCCESS);
    }else{
        printf("Please enter either a file or directory as the parameter\n");
    }
}