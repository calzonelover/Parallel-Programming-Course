#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
	int rank, size_process;
	int size_comm_process = 5;

	int i, dim[2], wrap[2], reorder = 1;
	
	MPI_Group group_world, group;
	MPI_Comm comm;
	
	// initialize MPI execution environment
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	dim[0] = dim[1] = 3; // then need num_all_processes = 9 to satisfy
	wrap[0] = wrap[1] = 1;
	
	MPI_Cart_create(MPI_COMM_WORLD, 2, dim, wrap, reorder, &comm);

	int coord[2], grid_rank;
	MPI_Comm_rank(comm, &grid_rank);	
	MPI_Cart_coords(comm, grid_rank, 2, coord);
	
	printf("rank %d: %d, %d \n", rank, coord[0], coord[1]);
	// terminate MPI execution environment
	MPI_Finalize();
	return 0;
}
