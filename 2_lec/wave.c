#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define xmax 1.0
#define nx 100
#define tmax 1.0
#define nt 100
#define a 1000.0
#define c 1.0

void init_wave(float *_u0, float *_u1);

void propagate_wave(float *_u0, float *_u1, float *_u2, FILE *_file);

int main(int argc, char** argv) {
  int ix, it;
  float dx = xmax/(nx+1);
  float *u0, *u1, *u2, dt, x;
  u0 = (float*) malloc(nx*sizeof(float));
  u1 = (float*) malloc(nx*sizeof(float));
  u2 = (float*) malloc(nx*sizeof(float));
  init_wave(u0, u1);
  FILE *file = fopen("uall.dat","w");
  propagate_wave(u0, u1, u2, file);
  fclose(file);
  return 0;
}

void init_wave(float *_u0,float *_u1){
    float dx = xmax/nx;
    float dt = tmax/nt;
    for (unsigned int ix=0; ix<nx; ix++){
        _u0[ix] = exp(-a*pow((ix*dx)-0.5*xmax, 2));
        _u1[ix] = exp(-a*pow((ix*dx)-0.5*xmax-c*dt, 2));
    }
}

void propagate_wave(float *_u0, float *_u1, float *_u2, FILE *_file){
	for (unsigned int it=0; it<nt; it++ ){
		for (unsigned int ix=0; ix<nx-1; ix++){
			_u2[ix] = _u1[ix+1]+_u1[ix-1]-_u0[ix];
			_u0[ix] = _u1[ix];
			_u1[ix] = _u2[ix];
		}
		fseek(_file, it*nx*sizeof(float), SEEK_SET);
		fwrite(_u2, sizeof(float), nx, _file);
	}
}
