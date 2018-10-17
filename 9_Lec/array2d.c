/* Example code for allocating memory for two-dimensional array.
 *
 * This code was written for teaching the course Parallel Programming (SCPY 571).
 *
 * Copyright 2014 Chaiwoot Boonyasiriwat. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define M 8192
// #define N 8192

int main(int argc, char *argv[]) {
  int M, N;
  M = N = atoi(argv[1]);
  printf("M = %d and N = %d \n", M, N);
  int i, j, **a;
  clock_t start, end;

  // Allocate memory for two-dimensional array
  a = malloc(sizeof(int*) * M);
  if (a != NULL)
    for (i = 0; i < M; i++)
      a[i] = malloc(sizeof(int) * N);
  else
    return 1;

  start = clock();
  // access along row direction first
  for (i=0; i<M; i++)
    for (j=0; j<N; j++)
      a[i][j] = i+j;
  end = clock();
  printf("Row access Time elapse = %lf ms\n",  (double) (end - start)/ CLOCKS_PER_SEC * 1000.0);

  start = clock();
  // access along column direction first
  for (j=0; j<N; j++)
    for (i=0; i<M; i++)
      a[i][j] = i+j;

  end = clock();
  printf("Column access Time elapse = %lf ms\n",  (double) (end - start)/ CLOCKS_PER_SEC *1000.0);

  // Free memory
  for (i = 0; i < M; i++)
    free(a[i]);
  free(a);

  return 0;
}