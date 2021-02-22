#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int main (void){
    int fd [2], fd_sum [2] , nbytes ;
    pid_t childpid ;
    int8_t kb_input = 0;
    int8_t readbuffer = 0;
    
    pipe (fd);
    pipe (fd_sum);
    if ((childpid = fork()) == -1){
        perror ("fork");
        exit (0);
    }
    if(childpid == 0){ // Child Running
        while(kb_input != -1){ //Keeps taking input until -1 is received
            close (fd [0]) ;
            scanf("%hhd", &kb_input); //take input and send to parent
            printf("kb_input: %hhd\n", kb_input);
            write (fd [1], &kb_input , sizeof(kb_input));
        }
        close(fd_sum[1]);
        int finalsum = 0; //init sum variable
        read(fd_sum[0], &finalsum, sizeof(finalsum)); //reads final sum after -1 received and prints
        printf("Sum of inputs: %d\n", finalsum);
        exit (0);
    }
    else { // Parent Running
        int sum = 0; //init sum
        do{ //Keeps taking input until -1 is received
            close (fd [1]);
            sum += (int) readbuffer; //add received value to sum
            nbytes = read (fd [0] , &readbuffer , sizeof (readbuffer)); //read new value
        }while(readbuffer != -1);
        close(fd_sum[0]); //-1 received, write sum to child
        write(fd_sum[1], &sum, sizeof(sum));
    }
    return 0;
}