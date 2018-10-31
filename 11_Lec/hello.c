#include <stdio.h>
#include <ifaddrs.h>
#include <mpi.h>

int main(int argc, char** argv) {
	int rank, n;
	// initialize MPI execution environment
	MPI_Init(&argc, &argv);
	// get the process rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	if (rank == 0)
		printf("number of processes = %d\n", n);
		
	struct ifaddrs *id;
	int val;
	val = getifaddrs(&id);
	
	printf("Hello from rank %d, pc=%s\n", rank, id->ifa_name);
	// terminate MPI execution environment
	MPI_Finalize();
	return 0;
}
