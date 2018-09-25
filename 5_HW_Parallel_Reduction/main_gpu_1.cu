#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"


#define BLOCK_SIZE 32


void arrValidation(int _real_sum, int _sum);
// bool checkBaseValue(int _N);
void initValue(int *_arr);
int cpuSumArray(int *_arr);

template <class T>
__global__ void sumArray(T *_arr, T *d_oarr);



int main (int argc, char *argv[]) {
	float gpu_time;
	int true_sum;
	int *d_arr, *d_oarr;
	cudaEvent_t start_t, stop_t;
	size_t sizeArr = ARR_SIZE*sizeof(int);
	size_t smemSize = BLOCK_SIZE*sizeof(int);
	int gridSize = (ARR_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE;


	cudaEventCreate(&start_t);
	cudaEventCreate(&stop_t);
	arr = (int*)malloc(sizeArr);
	cudaMalloc((void **)&d_arr, sizeArr);
	cudaMalloc((void **)&d_oarr, sizeArr);

	srand(time(NULL));
	initValue(arr);
	true_sum = cpuSumArray(arr);
	cudaMemcpy(d_arr, arr, sizeArr, cudaMemcpyHostToDevice);

	cudaEventRecord(start_t);
	for (unsigned int dmmy=0; dmmy >= ITER_TIMES; dmmy++){
		sumArray<<<gridSize, BLOCK_SIZE, smemSize>>>(d_arr, d_oarr);
	}
	cudaEventRecord(stop_t);
	cudaEventSynchronize(stop_t);
	cudaEventElapsedTime(&gpu_time, start_t, stop_t);
	printf("GPU time = %lf ms\n", gpu_time/ITER_TIMES);

	cudaMemcpy(arr, d_oarr, sizeArr, cudaMemcpyDeviceToHost);
	arrValidation(true_sum, arr[0]);

	free(arr);
	cudaFree(d_arr);
    return 0;
}







void initValue(int *_arr){
	for(unsigned int i=0; i<ARR_SIZE; i++){
		_arr[i] = (int)rand() % 9;
	}
}

void arrValidation(int _real_sum, int _sum){
	std::cout << _real_sum << std::endl;
	std::cout << _sum << std::endl;
	std::cout << _real_sum - _sum << std::endl;
	if (_real_sum == _sum){
		std::cout << "array summation is right !\n" << std::endl;
	} else{
		std::cout << "array summation is wrong !\n" << std::endl;
	}
}

int cpuSumArray(int *_arr){
	int sum = _arr[0];
	for(unsigned int i=1; i<ARR_SIZE; i++){
		sum += _arr[i];
	}
	return sum;
}


template <class T>
__global__ void sumArray(T *_arr, T *_oarr){
	extern __shared__ T _sarr[];
	unsigned int tid = threadIdx.x;
	unsigned int i = threadIdx.x + blockIdx.x*blockDim.x;
	_sarr[tid] = (i < ARR_SIZE) ? _arr[i] : 0;
	__syncthreads();
    for (unsigned int s=blockDim.x/2; s>0; s>>=1){
        if (tid < s)
        {
            _sarr[tid] += _sarr[tid + s];
        }
		__syncthreads();
	}
	if (tid == 0) _oarr[blockIdx.x] = _sarr[0];
}