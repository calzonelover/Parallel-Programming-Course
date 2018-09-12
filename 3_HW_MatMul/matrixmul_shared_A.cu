#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "matrixmul.h"

#define BLOCK_SIZE 32
#define STRIDE_SIZE 32

__global__ void matmul(float *_A, float *_B, float *_C);

int main(){
	float *hA, *hB, *hC;
	// time
	cudaEvent_t start, stop;
  	float gpu_time;
  	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(WB/BLOCK_SIZE+1, HA/BLOCK_SIZE+1);
	// allocate mem
	hA = (float*)malloc(WA*HA*sizeof(float));
	hB = (float*)malloc(WB*HB*sizeof(float));
	hC = (float*)malloc(WB*HA*sizeof(float));
	cudaMalloc((void **)&A, WA*HA*sizeof(float));
	cudaMalloc((void **)&B, WB*HB*sizeof(float));
	cudaMalloc((void **)&C, WB*HA*sizeof(float));
	// init value
	initMatrix(hA, WA, HA);
	initMatrix(hB, WB, HB);
	cudaMemcpy(A, hA, WA*HA*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(B, hB, WB*HB*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(C, hC, WB*HA*sizeof(float), cudaMemcpyHostToDevice);
	// check if there is no matching dim of matrix mul
	if (WA != HB){
		cudaFree(A);
		cudaFree(B);
		cudaFree(C);
		free(hA);
		free(hB);
		free(hC);
		exit(0);
		return 0;
	}
	cudaEventRecord(start);
	// compute and record
	for (unsigned int iter = 0; iter < MAX_ITER ; iter++)
		matmul<<<dimGrid, dimBlock>>>(A, B, C);
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&gpu_time, start, stop);
	printf("GPU time = %f s\n", gpu_time*0.001/MAX_ITER);
	cudaMemcpy(hA, A, WA*HA*sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(hB, B, WB*HB*sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(hC, C, WB*HA*sizeof(float), cudaMemcpyDeviceToHost);
	// printf("Matrix A =\n");
	// printMatrix(hA, WA, HA);
	// printf("Matrix B =\n");
	// printMatrix(hB, WB, HB);
	// printf("Matrix C =\n");
	// printMatrix(hC, WB, HA);
	cudaFree(A);
	cudaFree(B);
	cudaFree(C);
	free(hA);
	free(hB);
	free(hC);
	return 0;
}

void initMatrix(float *_M, int _W, int _H){
	srand(time(NULL));
	for (unsigned int h=0; h<_H;h++){
		for (unsigned int w=0; w<_W; w++){
			_M[w+h*_W] = (int)rand() % 16;//(float)rand()/ (float)RAND_MAX;
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


__global__ void matmul(float *_A, float *_B, float *_C){
	__shared__ float _Asub[STRIDE_SIZE][STRIDE_SIZE];
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if (i < WB && j < HA){
		_Asub[threadIdx.y][threadIdx.x] = _A[j*STRIDE_SIZE+threadIdx.x];
		float sumoverpad = 0.0f;
		for (unsigned int dmmy = 0; dmmy < STRIDE_SIZE; dmmy++){
			sumoverpad += _Asub[threadIdx.y][dmmy]*_B[i+dmmy*WB];
		}
		_C[i+j*WB] = sumoverpad;
	}
}