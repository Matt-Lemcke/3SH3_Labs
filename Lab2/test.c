#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int main (void){
    int fd [2] , nbytes ;
    pid_t childpid ;
    int8_t kb_input = 0;
    int8_t readbuffer = 0;
    int sum = 0;
    pipe (fd);
    if ((childpid = fork()) == -1){
        perror ("fork");
        exit (0);
    }

    if(childpid == 0){
        printf("child\n");
        while(kb_input != -1){
            scanf("%hhd", &kb_input);
            close (fd [0]);
            write (fd [1], &kb_input , sizeof(kb_input));
            printf("kb_input: %hhd\n", kb_input);
        }
        exit(0);
    }
    else {
        printf("parent\n");
        do{
            sum+=(int) readbuffer;
            close (fd[1]);
            nbytes = read (fd[0] , &readbuffer , sizeof(readbuffer));
            printf("readbuffer: %hhd\n", readbuffer);
        }while(readbuffer != -1);
        printf("sum: %d", sum);
    }
    return 0;
}