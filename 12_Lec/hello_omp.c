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

int main (int argc, char **argv) {
  int nthreads, tid, num;

//  omp_set_num_threads(4);
  printf("Maximum number of threads = %d\n", omp_get_max_threads());


  #pragma omp parallel private(nthreads, tid) shared(num)
  {
    // Get the thread number
    tid = omp_get_thread_num();
    
    num = 1;
    printf("thread %d: num = %d\n", tid, num);

    // Display the number of threads
    if (tid == 0)
    {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }

    printf("Hello World from thread %d\n", tid);
  }

  return 0;
}
