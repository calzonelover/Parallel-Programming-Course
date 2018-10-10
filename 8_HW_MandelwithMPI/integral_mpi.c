#include <stdio.h>
#include <mpi.h>

#include "integral.h"

void main(int argc, char *argv[]){
	float size, rank, number;
	MPI_Request request[N];
	MPI_STATUS status;

	MPI_INIT(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Number of comm_size (processes) = %d\n", size);
	float sum[N];

	if (rank == 0){
		// loop for sending all process
		for (unsigned int p=1; p<size; p++){
			MPI_Isend(&sum[p], 1, MPI_INIT, 1, p, MPI_COMM_WORLD, &request);
		}
		// loop for waiting
		for (unsigned int p=1; p<size; p++){
			MPI_Wait(&request, &status);
		}
	} else {

	}

	float result;
	result = integrate(X_I, X_F, N);
	printf("Result = %f\n", result);

	MPI_Finalize();
}

float func(float _x_in){
	return 1.0f;
}

float integrate(float _x_i, float _x_f){
	float dx = (float) (_x_f - _x_i) / N;
	float sum = 0.0f;
	float _x_now;
	for (unsigned int i=0; i<N; i++){
		_x_now = _x_i*dx;
		sum += func(_x_now)*dx;
	}
	return sum;
}





















