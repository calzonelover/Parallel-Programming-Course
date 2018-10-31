#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
	int rank, size_process;
	int size_comm_process = 5;
	
	int i;
	int *process_ranks;
	MPI_Group group_world;
	MPI_Group first_row_group;
	MPI_Comm comm;
	
	// initialize MPI execution environment
	MPI_Init(&argc, &argv);
	// get the process rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size_process);
	
	// make list of process
	process_ranks = (int*)malloc(size_comm_process*sizeof(int));
	for(unsigned int proc=0; proc<size_comm_process; proc++){
		process_ranks[proc] = proc;
	}
	// get world group
	MPI_Comm_group(MPI_COMM_WORLD, &group_world);
	// create new group
	MPI_Group_incl(group_world, size_comm_process, process_ranks, &first_row_group);
	// create new communicator
	MPI_Comm_create(MPI_COMM_WORLD, first_row_group, &comm);
	
	/*
	if (rank == 0)
		printf("number of processes = %d\n", size_process);
	printf("Hello from rank %d\n", rank);*
	*/
	
	/// program
	if (rank < 5)
		i = 1;
	else
		i = 0;
	if (rank == 0)
		i = 100;
	if (rank < 5)
		MPI_Bcast(&i, 1, MPI_INT, 0, comm);
	printf("rank %d has i %d \n", rank, i);
	
	MPI_Finalize();
	return 0;
}
