/* Hello World C program with OpenMP
 *
 * This code was written for Parallel Programming course
 * taught at Mahidol University.
 *
 * Copyright 2014 Chaiwoot Boonyasiriwat. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mandel.h"

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
   #define omp_get_num_threads() 1
   #define omp_get_max_threads() 1
#endif

int main (int argc, char **argv) {
    int iy_now, ix_now;


    int *map;
    map = malloc(sizeof(int)*NX*NY);

    float dx = (XF - XI)/NX;
    float dy = (YF - YI)/NY;

    int nthreads, num;

    printf("Maximum number of threads = %d\n", omp_get_max_threads());

    start_t = omp_get_wtime();
    #pragma omp parallel for private(iy_now, ix_now)
    for (unsigned int i=0; i<NY*NX; i++){
       ix_now = i % NX;
       iy_now = (i % NX > 0) ? (i-i%NX)/NX : i/NX;
       map[iy_now*NX + ix_now] = compute_mandel(ix_now, iy_now, dx, dy);
    }
    stop_t = omp_get_wtime();
    printf("Elapse time for static %lf \n", (stop_t - start_t));

    FILE *file;
    file = fopen("mandel_result_static.ppm", "w");
    fprintf(file, "P2 %d %d 16\n", NX, NY);
    for (unsigned int i=0; i<NX; i++){
        for (unsigned int j=0; j<NY; j++){
            fprintf(file, "%d \t", map[i+j*NX]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    fclose(file);

  return 0;
}

int compute_mandel(int _ix, int _iy, float _dx, float _dy){
    Complex z, z_prev, c;
    int k;
    float length_sq;
    z.x = z.y = 0.0f;
    k = 0;
    c.x = _dx*_iy + XI;
    c.y = _dy*_ix + YI;
    do {
        z_prev.x = z.x;
        z_prev.y = z.y;
        z.x = z_prev.x*z_prev.x - z_prev.y*z_prev.y + c.x;
        z.y = 2.0*z_prev.x*z_prev.y + c.y;
        length_sq = z.x*z.x + z.y*z.y;
        k++;
    } while (length_sq < (XF-XI + YF-YI) && k < MAX_ITER);
    if ( k >= MAX_ITER)
        k = 0;
    else
        k %= 16;
    return k;
}