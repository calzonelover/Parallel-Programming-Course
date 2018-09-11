#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "matrixmul.h"


void matmul(float *_A, float *_B, float *_C);

int main(){
	// int iter, max_iter = 20;
	A = (float*)malloc(WA*HA*sizeof(float));
	B = (float*)malloc(WB*HB*sizeof(float));
	C = (float*)malloc(WB*HA*sizeof(float));
	srand(time(NULL));
	initMatrix(A, WA, HA);
	initMatrix(B, WB, HB);
	start = clock();
	for (unsigned int iter = 0; iter < MAX_ITER ; iter++)
		matmul(A, B, C);
	stop = clock();
	double cpu_time = (double) (stop-start) / CLOCKS_PER_SEC / MAX_ITER;
	printf("CPU time = %lf s\n", cpu_time);
	// printf("Matrix A =\n");
	// printMatrix(A, WA, HA);
	// printf("Matrix B =\n");
	// printMatrix(B, WB, HB);
	// printf("Matrix C =\n");
	// printMatrix(C, WB, HA);
	free(A);
	free(B);
	free(C);
	return 0;
}

void initMatrix(float *_M, int _W, int _H){
	for (unsigned int h=0; h<_H;h++){
		for (unsigned int w=0; w<_W; w++){
			_M[w+h*_W] = (float)rand()/ (float)RAND_MAX;//(int)rand() % 16;
		}
	}
}

void matmul(float *_A, float *_B, float *_C){
	if (WA == HB){
		for (unsigned int j=0; j<HA;j++){
			for (unsigned int i=0; i<WB; i++){
				float sumover = 0.0f;
				for (unsigned int dmmy=0; dmmy<HB; dmmy++){
					sumover += _A[dmmy+j*WA]*_B[i+dmmy*WB]; // Have to swop
				}
				_C[i+j*WB] = sumover;
			}
		}
	} else {
		free(A);
		free(B);
		free(C);
		printf("Please give a match Dimension of multiplication matrix (Width A = Height B)\n");
		exit(0);
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