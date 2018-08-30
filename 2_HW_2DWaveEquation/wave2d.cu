#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wave2d.h"

__global__ void fd(float *u0, float *u1, float *u2){
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  int iy = threadIdx.y + blockIdx.y*blockDim.y;
  int indx = ix+iy*nx;
  if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1)
    u2[indx] = u1[indx+1]+u1[indx-1]-u0[indx];
}

__global__ void update(float *u0, float *u1, float *u2){
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  int iy = threadIdx.y + blockIdx.y*blockDim.y;
  int indx = ix+iy*nx;
  u0[indx] = u1[indx];
  u1[indx] = u2[indx];
}

int main(){
  cudaEvent_t start, stop;
  float gpu_time;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  size = nx*sizeof(float);
  float *h_u0, *h_u1;
  float dx = xmax/(nx-1);
  float dt = dx/v;
  h_u0 = (float*) malloc(size);
  h_u1 = (float*) malloc(size);
  cudaMalloc((void **)&u0, size);
  cudaMalloc((void **)&u1, size);
  cudaMalloc((void **)&u2, size);
  for (unsigned int ix=0; ix<nx; ix++){
    float x = ix*dx;
    h_u0[ix] = exp(-a*pow(x-0.5*xmax,2.0));
    h_u1[ix] = exp(-a*pow(x-0.5*xmax-v*dt,2.0));
  }
  cudaEventRecord(start, 0);
  cudaMemcpy(u0, h_u0, size, cudaMemcpyHostToDevice);
  cudaMemcpy(u1, h_u1, size, cudaMemcpyHostToDevice);
  for (unsigned int it=0; it<nt; it++) {
    fd<<<1,nx>>>(u0, u1, u2);
    update<<<1,nx>>>(u0, u1, u2);
  }
  cudaMemcpy(h_u0, u2, size, cudaMemcpyDeviceToHost);
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&gpu_time, start, stop);
  printf("GPU time = %f s\n", gpu_time*0.001);
  FILE *file = fopen("uGPU.dat","w");
  fwrite(h_u0, sizeof(float), nx, file);
  fclose(file);
  return 0;
}






