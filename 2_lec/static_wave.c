#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define xmax 1.0
#define nx 100
#define a 1000.0

void initwave(float *_u0);

int main() {
  int ix, it;
  float dx = xmax/(nx+1);
  float *u0, *u1, *u2, c, dt, x;
  u0 = (float*) malloc(nx*sizeof(float));
  u1 = (float*) malloc(nx*sizeof(float));
  u2 = (float*) malloc(nx*sizeof(float));
  initwave(u0);
  FILE *file = fopen("u0.dat","w");
  fwrite(u0, sizeof(float), nx, file);
  fclose(file);
  return 0;
}

void initwave(float *_u0){
    float dx = xmax/nx;
    for (unsigned int ix=0; ix<nx; ix++){
        _u0[ix] = exp(-a*pow((ix*dx)-0.5*xmax, 2));
    }
}
