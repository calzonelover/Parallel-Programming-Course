#include <stdio.h>
#include <mpi.h>

#include "integral.h"

int main(int argc, char *argv[]){
	int size, rank, number;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
		printf("Number of comm_size (processes) = %d\n", size);
	
	MPI_Request request[size-1];
	MPI_Status status[size-1];

	float sum[size-1];
	int sub_size = ((N & (size-1) != 0) && rank == size-1)? N/(size-1) + 1 : N/(size-1);
	float dx = (float) (X_F - X_I) / N;

	if (rank == 0){
		// send and wait
		for (unsigned int r=1; r<size; r++){
			MPI_Irecv(&sum[r-1], 1, MPI_FLOAT, r, r, MPI_COMM_WORLD, &request[r-1]);
		}
		MPI_Waitall(size-1, request, status);
		// check status
		for (unsigned int r=1; r<size; r++){
			printf("process rank %d status %d \n", r,status[r-1].MPI_ERROR);
			if (status[r-1].MPI_ERROR != MPI_SUCCESS)
				printf("Processes rank %d is fail\n", r);
		}
		// checking sum result
		float final_result;
		for (unsigned int r=1; r<size; r++){
			final_result += sum[r-1];
		}
		printf("Integral from MPI = %f \n", final_result);
	} else {
		float x_i = (float) X_I + dx*sub_size*(rank-1);
		float x_f = (float) X_I + dx*sub_size*(rank);
		float result_rank = integrate(x_i, x_f, dx);
		// send and wait
		for (unsigned int r=1; r<size; r++){
			MPI_Isend(&result_rank, 1, MPI_FLOAT, 0, r, MPI_COMM_WORLD, &request[r-1]);
		}
		for (unsigned int r=1; r<size; r++){
			MPI_Wait(&request[r-1], &status[r-1]);
		}
	}
  MPI_Finalize();
  return 0;
}

float func(float _x_in){
	return 1.0f*_x_in;
}

float integrate(float _x_i, float _x_f, float _dx){
	float _sum = 0.0f;
	float _x_now = _x_i + _dx/2.0f;
	while(_x_now < _x_f){
		_sum += func(_x_now)*_dx;
		_x_now += _dx;
	}
	return _sum;
}





















