/* References: https://www.classes.cs.uchicago.edu/archive/2017/winter/51081-1/LabFAQ/lab7/Semaphores.html
https://doc.lagout.org/programmation/unix/Linux%20System%20Programming%20Talking%20Directly%20to%20the%20Kernel%20and%20C%20Library.pdf
for IPC_CREAT | 0666: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(){
//Below are defs for file size and user inputs.
  struct stat fileInfo; // info about file in buffer.
  int resourceType;
  int resourceAmount;
//------------------------------------------------

//---------Below are semaphore defs------------
  key_t Key = 6969;
  int semFlag = IPC_CREAT | 0666;
  /* the above create a new semaphore;
  0666 is the usual access permision in linux
  in rwx octal format and having the sequence
*/
  int newSem = 1; //1 new semaphore created
  int semID = semget(Key, newSem, semFlag); //semaphore ID

  //setting semaphore to 1:
  semctl(semID, 0, SETVAL, 1);
//--------------------------------------------------

//----Defining incrementing and decrementing operations------
  struct sembuf incrementing;
  incrementing.sem_num = 0;
  incrementing.sem_op = 1;
  incrementing.sem_flg = SEM_UNDO;

  struct sembuf decrementing;
  decrementing.sem_num = 0;
  decrementing.sem_op = -1;
  decrementing.sem_flg = SEM_UNDO;
//----------------------------------------------------------
  int nsops = 1;

/*Open a text file for update
  r+ deletes the content nor
  creates a new file if it doesn't exist */
  FILE *resourceFile = fopen("test.txt", "r+");

  fstat(fileno(resourceFile), &fileInfo);
  int dataLength = fileInfo.st_size; //size of file
  int fileNum = fileno(resourceFile); //return the integer file descriptor associated with the stream pointed to by stream.
//Above 3 lines get the file information

  //mapping to memory region..
  char *region = mmap(NULL, dataLength, PROT_WRITE | PROT_READ, MAP_SHARED, fileNum, 0);

  //Alocating resources..
  while(1){
	fwrite(region, 1, buffer.st_size, stdout);
    printf("\nCurrent Resources:\n");
    printf("%s\n", region);

    printf("Resource needed: ");
    scanf("%d", &resourceType);

    printf("Amount of resource needed: ");
    scanf("%d", &resourceAmount);

    //entering critical section...
    semop(semID, &decrementing, nsops);
    /*semop() performs operations on selected semaphores in the set indi‐
       cated by semid.*/


    //Deducting resources
    char a = region[resourceType*4+2];
    char b[2];
    b[0] = a;
    b[1] = '\0';
    // printf("HELLO%c ; %c \n", a,b[0]);
    int availableResources = atoi(b);
    // printf("WORLD%d\n", availableResources);
    printf("\nResource Type: %d\n",resourceType);
    printf("Amount available: %d\n",availableResources);


    if (availableResources >= resourceAmount){
      //calculate amount of resource remaining after deduction
        int newAmount = availableResources - resourceAmount;
        char nA[2];
		snprintf(nA, 1, "%d", newAmount);
        // sprintf(&nA, "%d", newAmount);
        region[resourceType*4+2] = nA;
        printf("\nResource Type: %d\n", resourceType);
        printf("Amount allocated: %d\n", resourceAmount);

        //syncing...
        msync(region, dataLength, MS_SYNC);
    }
    else{    //Error message..

        printf("Unable to allocate resource.\n");
    }
    //Leaving critical section...
    semop(semID, &incrementing, nsops);
  }

return 0;
}