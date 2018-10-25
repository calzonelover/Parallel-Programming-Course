#include <stdio.h>
#include <mpi.h>

#define N_PARTICLES 2

#define X_I -1.0
#define X_F 1.0
#define Y_I -1.0
#define Y_F 1.0

#define TAG_DONE -1

typedef struct{
	float x, y, z;
	float m;
} Particle;

void main(int argc, char** argv) {
	int rank, n;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	if (rank == 0)
		printf("number of processes = %d\n", n);
	printf("Hello from rank %d\n", rank);

	MPI_Datatype MPI_PARTICLE;
	MPI_Type_contiguous(4, MPI_FLOAT, &MPI_PARTICLE);
	MPI_Type_commit(&MPI_PARTICLE);

	Particle myparticles[N_PARTICLES];
	if (rank == 0){
		for (unsigned int i = 0; i < N_PARTICLES; i++)
		{
			myparticles[i].x = myparticles[i].y = myparticles[i].z = 0.1f;
			myparticles[i].m = 2.0f;
		}
	}
	// Loop for small dt
		// MPI Broadcast myparticles from root
		MPI_Bcast(&myparticles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);
		// master-slave loop over each one particle
			// M: send indx particle to slave
			// S: update position particle and send to master
			// M: receive myparticles[indx] to root
		// save file in some step time	

	if (rank != 0){
		printf("rank %d got mass_0 %f mass_1 %f\n", rank, myparticles[0].m, myparticles[1].m);
	}

	MPI_Finalize();
}