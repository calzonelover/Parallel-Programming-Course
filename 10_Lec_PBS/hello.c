#include <stdio.h>
#include <mpi.h>

void main(int argc, char** argv) {
	int rank, n;
	// initialize MPI execution environment
	MPI_Init(&argc, &argv);
	// get the process rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	if (rank == 0)
		printf("number of processes = %d\n", n);
	printf("Hello from rank %d\n", rank);
	// terminate MPI execution environment
	MPI_Finalize();
}