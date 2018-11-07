/* Hello World C program with OpenMP
 *
 * This code was written for Parallel Programming course
 * taught at Mahidol University.
 *
 * Copyright 2014 Chaiwoot Boonyasiriwat. All rights reserved.
 */

#include <stdio.h>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
   #define omp_get_num_threads() 1
   #define omp_get_max_threads() 1
#endif

void whoiam();


int main (int argc, char **argv) {
  int nthreads, tid, num;

  printf("Maximum number of threads = %d\n", omp_get_max_threads());

  #pragma omp parallel
  {
      whoiam();
  }

  return 0;
}


void whoiam(){
    int _tid = omp_get_thread_num();
    printf("Hello from tid %d \n", _tid);
}
