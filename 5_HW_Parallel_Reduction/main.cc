#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"

int sumArray(int *_arr);

int main () {
	clock_t start_t, stop_t;
	arr = (int*)malloc(ARR_SIZE*sizeof(int));
	initValue(arr);
	start_t = clock();
	for (unsigned int dmmy=0; dmmy < ITER_TIMES; dmmy++){
		int sum = sumArray(arr);
	}
	stop_t = clock();
	double cpu_time = (double) (stop_t-start_t) / CLOCKS_PER_SEC / ITER_TIMES * 1000.0f;
	printf("CPU time = %lf ms\n", cpu_time);
	free(arr);
    return 0;
}


int sumArray(int *_arr){
	int sum = arr[0];
	for(unsigned int i=1; i<ARR_SIZE; i++){
		sum += _arr[i];
	}
	return sum;
}


void initValue(int *_arr){
	srand(time(NULL));
	for(unsigned int i=0; i<ARR_SIZE; i++){
		_arr[i] = (int)rand()/ (int)RAND_MAX;
	}
}