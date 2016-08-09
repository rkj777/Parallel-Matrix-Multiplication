#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "lab1_IO.c"

/* Decaring variables used by threads */
int** A;
int** B;
int** C;
int n;
int num_threads;

int matrixMultiply(int row, int col);
void* Worker(void* rank);

int main(int argc, char* argv[]){
  if(argc != 2){
    printf("Wrong! Error please provide number of threads");
    return -1;
  }
	
  /* Start timer */
  double start;
  GET_TIME(start);

  /* Get number of threads */
  num_threads = atoi(argv[1]);
  pthread_t* threadpool;
  
  /* Load generated matrix file */
  Lab1_loadinput(&A, &B, &n);

  /* initalizing threads */
  threadpool = malloc(num_threads *sizeof(pthread_t));
	
  /* Initalize result matrix */
  C = malloc(n * sizeof(int*));
  int i = 0;
  for (; i < n; i++){
    C[i] = malloc(n * sizeof(int));
  }

  /* Create threads  */
  int t;
  for(t = 0; t < num_threads; t++){
    pthread_create(&threadpool[t], NULL, Worker, (void*) t);
  }

  /* Wait for all threads to finish  */
  int t2;
  for(t2 = 0; t2 < num_threads; t2++){
    int result = pthread_join(threadpool[t2],NULL);
    if(result){
      printf("ERROR; return code from pthread_join() is %d\n",
	     result);
      exit(-1);
    }	
  }

  /* Get final time and calculate delta  */
  double end;
  GET_TIME(end);
  double final = end - start;
  printf("Running time %lf\n", final);

  Lab1_saveoutput(C,&n,end-start);
	
	
  /* Free memory for matricies  */
  int f;			
  for (f = 0; f< n; f++){
    free(A[f]);
    free(B[f]);
    free(C[f]);
  }
  free(A);
  free(B);
  free(C);

  return(1);
}

/* Thread Function  */
void* Worker(void* rank){

  /* Get bounds on thread work */
  int k = (int)rank;
  int x = k/sqrt((double)num_threads);
  int y = k % (int)sqrt(num_threads);
  int lower = n/sqrt(num_threads); 
  int xupper = (lower*(x +1)) - 1; 
  int yupper = (lower*(y +1)) - 1; 
  int xlower = lower * x; 
  int ylower = lower * y; 

  int i = xlower;
  int j = ylower;
  for(; i <= xupper; i++){
    for(j = ylower; j <= yupper; j++){
      C[i][j] = matrixMultiply(i,j);
    }  
  }	
  return NULL;
  pthread_exit(NULL);			
}

/* Determines matrix product for a single element */
int matrixMultiply(int row, int col){
  int P = 0;
  int index = 0;
  for(; index < n; index++){
    P += A[row][index] * B[index][col];
  }
  return P;		
}

