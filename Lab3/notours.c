/* @file lab3.c
  @date 25.02.2020
  @brief
  Performing Shear sort using Pthreads
  USAGE: gcc -pthread lab3.c
  references: http://www.skrenta.com/rt/man/pthread_mutex_init.3.html
              https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html
              https://francium.cc/blog/an-implementation-of-parallel-shearshort-in-c-using-pthreads/?fbclid=IwAR1XmkixaOtcEXAKyK607icVw_DBZIEpoFS5NDDeBCKHdRBx8HifdAd89Lk
ASSUMPTION: the input has only one NxN matrix in the file.
*/
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <string.h>
# include <math.h>
# include <pthread.h>

int n = 0; //matrix dimension
int **matrix;
pthread_mutex_t Mutex; //mutex ==> MUtual EXclusion
pthread_cond_t threshold;
int thread_counter = 0;
/* @brief Shear_sort implementation
   @details
      -implementation of Shear sort.
      -row-wise and column-wise sorting is done by bubble sort.
 */
void *Shear_sort(void *i){

  int id = *(int*) i;
  int j, k, l;
  for(j = 0; j <= n; j++){
      printf("Phase: %d\n", j);
      if((j%2) == 0){
          if((id%2) == 0){
              // Implementing Forward Row Sort
              int a, b, temp = 0;
              int row = id;
              //Here we're using bubble sort
              for(a = 0; a < n-1; a++){
                  for(b = 0; b < n-a-1; b++){
                      if(matrix[row][b] > matrix[row][b+1]){
                          temp = matrix[row][b+1];
                          matrix[row][b+1] = matrix[row][b];
                          matrix[row][b] = temp;
                      }
                  }
              }
              // Forward sort completed here!!!
          }
          else{
              // Implementing Reverse Row Sort
              int a, b, temp = 0;
              int row = id;
              // printf("Starting Reverse Row Sort, thread(%d)\n", id);
              //Here we're using bubble sort
              for(a = 0; a < n-1; a++){
                  for(b = 0; b < n-a-1; b++){
                      if(matrix[row][b] < matrix[row][b+1]){
                          temp = matrix[row][b+1];
                          matrix[row][b+1] = matrix[row][b];
                          matrix[row][b] = temp;
                      }
                  }
              }
              // Reverse sort completed here!!!
          }
      }
    else{
          // Implementing Column Sort
          int a, b, temp = 0;
          int col = id;
          //Here we're using bubble sort
          for(a = 0; a < n-1; a++){
              for(b = 0; b < n-1-a; b++){
                  if(matrix[b][col] > matrix[b+1][col]){
                      temp = matrix[b][col];
                      matrix[b][col] = matrix[b+1][col];
                      matrix[b+1][col] = temp;
                  }
              }
          }
          //Done Column Sort.
      }
      // create new thread
      pthread_mutex_lock(&Mutex); //locks the mutex
      thread_counter++;
  if(thread_counter < n){
          // Waitng until all threads complete...
          pthread_cond_wait(&threshold, &Mutex);
      }
    else{
          pthread_cond_broadcast(&threshold);//Unblocks all the threads.
          thread_counter = 0;
          //printing the matrix after each phase sort
          for(k = 0; k < n; k++){
              for(l = 0; l < n; l++){
                  printf("%d ", matrix[k][l]);
              }
              printf("\n");
          }
      }
      pthread_mutex_unlock(&Mutex);
  }
  pthread_exit(NULL);
}


/* Here, we shall read a file, sort it using shearshort and
write the output to another file.
*/
int main(){
  int i, j, k;
    /* in_size is used to measure the number of rows of the matrix,
    while in_mat is used to read and initialise the matrix.*/
  FILE *in_size, *in_mat;
  in_size = fopen("input.txt", "r");
  in_mat = fopen("input.txt", "r");
  char ch;

//Getting the number of rows.
  while((ch = fgetc(in_size))!= EOF){
    if (ch == '\n') n++;
  }
  n++;
  //now we know the dimension of our square matrix.
  int thread_ids[n];
  //Let's set up some threads!!
    pthread_t threads[n];
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE); //all created threads in joinable state
    pthread_cond_init(&threshold, NULL);
    pthread_mutex_init(&Mutex, NULL);


  //Here we are first allocating memory for the matrix.
  //This way we do not need to hardcode our 'n' value.
matrix = (int**)malloc(sizeof(int*) * n);
for (int a = 0; a<n; a++){
  matrix[a] = malloc(sizeof(int) * n);
}

//Now we can create and initialise the matrix.
  for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
          fscanf(in_mat, "%d", &matrix[i][j]);
      }
  }


  printf("Input Matrix is:\n");
  for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
          printf("%d ", matrix[i][j]);
      }
      thread_ids[i] = i; //This is a hack-job; just to initialise the thread thread_ids[].
      printf("\n");
  }

  for(i = 0; i < n; i++){
      pthread_create(&threads[i], &attribute, Shear_sort, &thread_ids[i]);
  }

  for(int b = 0; b < n; b++){
          pthread_join(threads[b], NULL);
      }//Threads joined after sorting.

  //The closing destruction
  fclose(in_mat); fclose(in_size);
  pthread_attr_destroy(&attribute);
  pthread_cond_destroy(&threshold);
  pthread_mutex_destroy(&Mutex);
  
printf("------------------------------------------------------------------\n");
printf("-----------------------------SORTING DONE!!!----------------------\n");
printf("------------------------------------------------------------------\nFINAL MATRIX: \n");
  for(j = 0; j < n; j++){
      for(k = 0; k < n; k++){
          printf("%d ", matrix[j][k]);
      }
      printf("\n");
  }

  pthread_exit(NULL);

  return 0;
}