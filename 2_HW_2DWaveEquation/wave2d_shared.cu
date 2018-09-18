#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wave2d.h"

#define BLOCK_SIZE 32

__global__ void fd(float *u0, float *u1, float *u2, float C2);

__global__ void update(float *u0, float *u1, float *u2);

int main(){
  cudaEvent_t start, stop;
  float gpu_time;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  float *h_u0, *h_u1;
  float dx = xmax/(nx-1);
  C = v*dt/dx;
  C2 = C*C;
  size = nx*ny*sizeof(float);
  dim3 B(BLOCK_SIZE,BLOCK_SIZE);
  dim3 G(nx/BLOCK_SIZE+1, ny/BLOCK_SIZE+1);
  h_u0 = (float*) malloc(size);
  h_u1 = (float*) malloc(size);
  cudaMalloc((void **)&u0, size);
  cudaMalloc((void **)&u1, size);
  cudaMalloc((void **)&u2, size);
  for (unsigned int iy=0; iy<ny; iy++){
    float y_now = iy*dx - 0.5*ymax;
    for (unsigned int ix=0; ix<nx; ix++){
        int indx = ix+iy*nx;
        float x_now = ix*dx - 0.5*xmax;
        h_u0[indx] = exp(-a*(pow(x_now,2)+pow(y_now,2)));
        h_u1[indx] = h_u0[indx];
    }
  }
  cudaMemcpy(u0, h_u0, size, cudaMemcpyHostToDevice);
  cudaMemcpy(u1, h_u1, size, cudaMemcpyHostToDevice);
  cudaEventRecord(start, 0);
  for (unsigned int it_time=0; it_time<MAX_ITER; it_time++){
    for (unsigned int it=0; it<nt; it++) {
      fd<<<G,B>>>(u0, u1, u2, C2);
      update<<<G,B>>>(u0, u1, u2);
    }
  }
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&gpu_time, start, stop);
  printf("GPU time = %f s\n", gpu_time*0.001/MAX_ITER);
  cudaMemcpy(h_u0, u2, size, cudaMemcpyDeviceToHost);
  FILE *file = fopen("uGPU_shared.dat","w");
  fwrite(h_u0, sizeof(float), nx*ny, file);
  fclose(file);
  //
  free(h_u0);
  free(h_u1);
  cudaFree(u0);
  cudaFree(u1);
  cudaFree(u2);
  return 0;
}


__global__ void fd(float *u0, float *u1, float *u2, float C2){
  __shared__ float _u0[BLOCK_SIZE][BLOCK_SIZE];
  __shared__ float _u1[BLOCK_SIZE][BLOCK_SIZE];
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  int iy = threadIdx.y + blockIdx.y*blockDim.y;
  int indx = ix+iy*nx;
  if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1){
    _u0[threadIdx.y][threadIdx.x] = u0[iy*BLOCK_SIZE+threadIdx.x];
    _u1[threadIdx.y][threadIdx.x] = u1[threadIdx.y*nx+ix];
  } else {
    _u0[threadIdx.y][threadIdx.x] = 0.0f;
    _u1[threadIdx.y][threadIdx.x] = 0.0f;
  }
  __syncthreads();
  if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1)
    u2[indx] = (2.0f-4.0f*C2)*_u1[ix][iy] - _u0[ix][iy]
              + C2*(_u1[ix+1][iy]+_u1[ix-1][iy]
              + _u1[ix][iy+1]+_u1[ix][iy-1]);
}


__global__ void update(float *u0, float *u1, float *u2){
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  int iy = threadIdx.y + blockIdx.y*blockDim.y;
  if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1){
   int indx = ix+iy*nx;
   u0[indx] = u1[indx];
   u1[indx] = u2[indx];
  }
}



