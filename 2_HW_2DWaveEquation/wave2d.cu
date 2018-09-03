#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wave2d.h"


__global__ void fd(float *u0, float *u1, float *u2, float C2){
  int ix = threadIdx.x + blockIdx.x*blockDim.x;
  int iy = threadIdx.y + blockIdx.y*blockDim.y;
  int indx = ix+iy*nx;
  if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1)
    u2[indx] = (2.0f-4.0f*C2)*u1[indx] - u0[indx]
              + C2*(u1[(ix+1)+iy*nx]+u1[(ix-1)+iy*nx]
              + u1[ix+(iy+1)*nx]+u1[ix+(iy-1)*nx]);
    //u1[indx+1]+u1[indx-1]-u0[indx];
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
  dim3 B(32,32);
  dim3 G(nx/32+1, ny/32+1);
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
  cudaEventRecord(start, 0);
  cudaMemcpy(u0, h_u0, size, cudaMemcpyHostToDevice);
  cudaMemcpy(u1, h_u1, size, cudaMemcpyHostToDevice);
  for (unsigned int it=0; it<nt; it++) {
    fd<<<G,B>>>(u0, u1, u2, C2);
    update<<<G,B>>>(u0, u1, u2);
  }
  cudaMemcpy(h_u0, u2, size, cudaMemcpyDeviceToHost);
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&gpu_time, start, stop);
  printf("GPU time = %f s\n", gpu_time*0.001);
  FILE *file = fopen("uGPU.dat","w");
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






