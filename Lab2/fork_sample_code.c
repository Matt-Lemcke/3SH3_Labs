#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
int test (){
    if ( fork () == 0) printf ("This is the child process\n");
    else {printf ("I am the parent process\n")};
    printf ("Both parent and child reach here.\n");
    return 0;
}

int main(){
    for(int i = 0; i<10; i++){
        test();
        printf("\n ------------- \n");
    }
}