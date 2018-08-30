/* Solving the 2D acoustic wave equation using explicit finite
 * difference method
 * Copyright 2018 Chaiwoot Boonyasiriwat. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "wave2d.h"
#define dt 0.035

int main() {
  float dx = xmax/(nx-1);
  float C = v*dx/dt;
  float C2 = C*C;
  size = nx*ny*sizeof(float);
  u0 = (float*) malloc(size);
  u1 = (float*) malloc(size);
  u2 = (float*) malloc(size);
  for (unsigned int iy=0; iy<ny; iy++) {
    float yy = iy*dx - 0.5*ymax;
    for (unsigned int ix=0; ix<nx; ix++) {
      int indx = ix+iy*nx;
      float xx = ix*dx - 0.5*xmax;
      u0[indx] = exp(-a*(pow(xx,2)+pow(yy,2)));
      u1[indx] = u0[indx];
    }
  }
  start = clock();
  for (unsigned int it=0; it<nt; it++) {
    // advance wavefields at inner nodes
    for (unsigned int iy=1; iy<ny-1; iy++) {
      for (unsigned int ix=1; ix<nx-1; ix++) {
        int indx = ix+iy*nx;
        u2[indx] = (2.0f-4.0f*C2)*u1[indx] - u0[indx]
          + C2*(u1[(ix+1)+iy*nx]+u1[(ix-1)+iy*nx]
              +u1[ix+(iy+1)*nx]+u1[ix+(iy-1)*nx]);
      }
    }

    // update
    for (unsigned int iy=0; iy<ny; iy++) {
      for (unsigned int ix=0; ix<nx; ix++) {
        int indx = ix+iy*nx;
        u0[indx] = u1[indx];
        u1[indx] = u2[indx];
      }
    }
  }
  stop = clock();
  double cpu_time = (double) (stop-start) / CLOCKS_PER_SEC;
  printf("CPU time = %lf s\n", cpu_time);

  // output the final snapshot
  FILE *file = fopen("u.dat","w");
  fwrite(u2, sizeof(float), nx*ny, file);
  fclose(file);

  // Free memory
  free(u0);
  free(u1);
  free(u2);

  return 0;
}

