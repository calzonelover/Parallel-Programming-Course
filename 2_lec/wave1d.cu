#include <stdio.h>
#include <stdlib.h>
#include <math.h>
__host__ void cpuFunction(){}
__device__ void gpuFunction(){}
__global__ void fd(float *u0, float *u1, float *u2, int nx) {
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  if (ix > 0 && ix < nx-1)
    u2[ix] = u1[ix+1]+u1[ix-1]-u0[ix];
}
__global__ void update(float *u0, float *u1, float *u2) {
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  u0[ix] = u1[ix];
  u1[ix] = u2[ix];
}
int main() {
  int ix, it, nx = 100, nt = 100;
  size_t size = nx*sizeof(float);
  float xmax = 1.0, dx = xmax/(nx+1);
  float *hU0, *hU1;
  float *u0, *u1, *u2, c = 0.1, dt = dx/c, x, a = 1000;
  hU0 = (float*) malloc(size);
  hU1 = (float*) malloc(size);
  cudaMalloc((void **)&u0, size);
  cudaMalloc((void **)&u1, size);
  cudaMalloc((void **)&u2, size);
  for (ix=0; ix<nx; ix++) {
    x = ix*dx;
    hU0[ix] = exp(-a*pow(x-0.5*xmax,2.0));
    hU1[ix] = exp(-a*pow(x-0.5*xmax-c*dt,2.0));
  }
  cudaMemcpy(u0, hU0, size, cudaMemcpyHostToDevice);
  cudaMemcpy(u1, hU1, size, cudaMemcpyHostToDevice);
  for (it=0; it<nt; it++) {
    fd<<<1,nx>>>(u0, u1, u2, nx);
    update<<<1,nx>>>(u0, u1, u2);
  }
  cudaMemcpy(hU0, u2, size, cudaMemcpyDeviceToHost);
  FILE *file = fopen("u.dat","w");
  fwrite(hU0, sizeof(float), nx, file);
  fclose(file);
  return 0;
}
