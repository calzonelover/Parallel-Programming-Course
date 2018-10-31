#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
	int rank, size_process;
	int size_comm_process = 5;
	
	int i;
	
	int *process_ranks;
	MPI_Group group_world;
	MPI_Group group;
	MPI_Comm comm;
	
	// initialize MPI execution environment
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	
	if (rank < 5)
		i = 0;
	else
		i = 1;
	MPI_Comm_split(MPI_COMM_WORLD, i, rank, &comm);
	MPI_Comm_size(comm, &size_process);
	
	if (rank == 0) printf("size = %d \n", size_process);
	if (rank == 0) i = 100;
	MPI_Bcast(&i, 1, MPI_INT, 0, comm);
	printf("rank %d: i = %d\n", rank, i);
	
	
	// terminate MPI execution environment
	MPI_Finalize();
	return 0;
}
