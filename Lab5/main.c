#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>



int fileInfo(char *input){
    struct stat buffer;
    if (stat(input, &buffer) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    int adjustedMode = buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    printf("Mode:                     %o\n", adjustedMode);
    printf("Link count:               %ld\n", (long) buffer.st_nlink);
    
    struct passwd *name;
    name = getpwuid(buffer.st_uid);
    if(name == NULL){
        perror("Error, cannot obtain owner name");
        return 0;
    }
    printf("Owners Name:              %s\n", name->pw_name);
    
    struct group *group;
    group = getgrgid(buffer.st_gid);
    if(group == NULL){
        perror("Error, cannot obtain owner name");
        return 0;
    }
    printf("Group Name:               %s\n", group->gr_name);
    printf("File size:                %lld bytes\n", (long long) buffer.st_size);
    printf("Blocks allocated:         %lld\n", (long long) buffer.st_blocks);
    printf("Last file modification:   %s", ctime(&buffer.st_mtime));
    printf("Name:                     %s\n", input);

    return(S_ISDIR(buffer.st_mode));
}

void traverse(char *input){
    // opendir(3C), readdir(3C), closedir(3C), chdir(2), getcwd(3C) and rewinddir(3C)
    DIR *dirp;
    char cwd[200];
    struct dirent *dp;
    char* file;
    if ((dirp = opendir(input)) == NULL) {
        perror("couldn't open directory");
        return;
    }

    getcwd(cwd, sizeof(cwd));

    if(chdir(input) == -1){
        perror("couldn't enter directory");
    }

    while(!(dp = readdir(dirp)==NULL)){
        if(!(strcmp(dp->d_name, "." == 0) || strcmp(dp->d_name, ".." == 0))){
            file = dp->d_name;
            int isDir = fileInfo(file);
            if(isDir){
                // DIR
                traverse(file)
                
            }else{
                printf("is a file");
                // FILE
            }
        }
    }

    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
                if (dp->d_name != 0)
                        continue;

                (void) printf("found\n");
                (void) closedir(dirp);
                return;
        }
    } while (dp != NULL);

    if (errno != 0)
            perror("error reading directory");
    else
            (void) printf("failed to find\n");
    (void) closedir(dirp);
    return;
}

int main(int argv, char *argc[]){
    if(argv == 2){
        char* inputName = argc[1];
        int isDir = fileInfo(inputName);
        printf("\n");
        if(isDir){
            printf("Is a directory\n");
            traverse(inputName);
            // Traverse Directory Function

        }
        else{
            printf("Is a file\n");
            // Just print file info!
        }
    }
    else{
            printf("Please enter either a file or directory as the parameter\n");
            return 0;
    }
}


