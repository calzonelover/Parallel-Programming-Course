#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "matrixmul.h"

#define BLOCK_SIZE 32
__global__ void matmu(float *_A, float *_B, float *_C);


int main(){
	float *hA, *hB, *hC;
	// time
	cudaEvent_t start, stop;
  	float gpu_time;
  	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	dim3 B(BLOCK_SIZE, BLOCK_SIZE);
	dim3 G(nx/BLOCK_SIZE+1, ny/BLOCK_SIZE+1);
	// host
	hA = (float*)malloc(WA*HA*sizeof(float));
	hB = (float*)malloc(WB*HB*sizeof(float));
	hC = (float*)malloc(WB*HA*sizeof(float));
	srand(time(NULL));
	initMatrix(hA, WA, HA);
	initMatrix(hB, WB, HB);
	// device
	cudaMalloc((void **)&A, size);
	cudaMalloc((void **)&B, size);
	cudaMalloc((void **)&C, size);
	cudaMemcpy(A, hA, size, cudaMemcpyHostToDevice);
	cudaMemcpy(B, hB, size, cudaMemcpyHostToDevice);
	cudaMemcpy(C, hC, size, cudaMemcpyHostToDevice);
	// compute and record
	cudaEventRecord(start, 0);
	matmul<<<G, B>>>(A, B, C);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&gpu_time, start, stop);
	printf("GPU time = %f s\n", gpu_time*0.001);
	cudaMemcpy(A, hA, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(B, hB, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(C, hC, size, cudaMemcpyDeviceToHost);
	// printf("Matrix A =\n");
	// printMatrix(A, WA, HA);
	// printf("Matrix B =\n");
	// printMatrix(B, WB, HB);
	// printf("Matrix C =\n");
	// printMatrix(C, WB, HA);
	cudaFree(A);
	cudaFree(B);
	cudaFree(C);
	free(hA);
	free(hB);
	free(hC);
	return 0;
}

void initMatrix(float *_M, int _W, int _H){
	for (unsigned int h=0; h<_H;h++){
		for (unsigned int w=0; w<_W; w++){
			_M[w+h*WA] = (float)rand()/ (float)RAND_MAX;//(int)rand() % 16;
		}
	}
}

void printMatrix(float *_M, int _W, int _H){
	for (unsigned int h=0; h<_H;h++){
		printf("%d|\t", h);
		for (unsigned int w=0; w<_W; w++){
			printf("%f\t", _M[w+h*_W]);
		}
		printf("|\n");
	}
}

__global__ void matmu(float *_A, float *_B, float *_C){
	__shared__ float sumover;
	int ix = threadIdx.x + blockIdx.x*blockDim.x;
	int iy = threadIdx.y + blockIdx.y*blockDim.y;
	int indx = ix+iy*nx;
	if (ix > 0 && iy > 0 && i < HA && j < WB){
		float sumover = 0.0f;
		for (unsigned int dmmy = 0; dmmy< HB; dmmy++){
			sumover += _A[dmmy+i*WA]*_B[j+dmmy*WB];
		}
		_C[j+i*WB] = sumover;
	}
}