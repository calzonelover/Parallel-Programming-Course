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
  int sum = 0;
  int N = 21;

    printf("Maximum number of threads = %d\n", omp_get_max_threads());
  
    #pragma omp parallel for reduction(+:sum) if (N >= 20)
    for (unsigned int i=0; i<N; i++){
        tid = omp_get_thread_num();
        sum += i;
        printf("Loop index %d from thread %d\n", i, tid);
    }
    printf("sum = %d\n", sum);

  return 0;
}
