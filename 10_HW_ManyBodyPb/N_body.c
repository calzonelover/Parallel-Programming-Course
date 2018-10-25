#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>

#define N_PARTICLES 2

#define STEP_TIME 0.0001
#define NT 1000

#define X_I -1.0
#define X_F 1.0
#define Y_I -1.0
#define Y_F 1.0

#define V_MAX 0.2

#define TAG_DONE -1
#define TAG_UPDATE_POSITION -2
#define TAG_UPDATE_VELOCITY -3

typedef struct{
	float x, y;
	float vx, vy;
	float m;
} Particle;

void main(int argc, char** argv) {
	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
		printf("number of processes = %d\n", size);
	printf("Hello from rank %d\n", rank);

	MPI_Datatype MPI_PARTICLE;
	MPI_Type_contiguous(5, MPI_FLOAT, &MPI_PARTICLE);
	MPI_Type_commit(&MPI_PARTICLE);

	// initialize N particles
	Particle particles[N_PARTICLES];
	if (rank == 0){
		for (unsigned int i = 0; i < N_PARTICLES; i++)
		{
			particles[i].x = particles[i].y = 0.1f;
			particles[i].m = 0.8f;
		}
	}
	/// Master
	if (rank == 0){
		int num_i, num_sent = 0;
		int p_i;
		MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);
		// Loop for step time
		for (unsigned int t_i=0; t_i < NT; t_i++){
			/* 
				Update position
			*/
			// first send to slave
			for (unsigned int r_i=1; r_i<size; r_i++){
				num_i = num_sent++;
				MPI_Send(&num_i, 1, MPI_INT, MPI_ANY_SOURCE, TAG_UPDATE_POSITION, MPI_COMM_WORLD);
			}
			for (unsigned int p_i=0; p_i < N_PARTICLES - size; p_i++){
				// receive myparticles[indx] to root
				MPI_Recv(&particles[t_i], 1, MPI_PARTICLE, MPI_ANY_SOURCE, TAG_UPDATE_POSITION, MPI_COMM_WORLD, &status);
				// send indx particle to slave
				num_i = num_sent++;
				MPI_Send(&num_i, 1, MPI_INT, MPI_ANY_SOURCE, TAG_UPDATE_POSITION, MPI_COMM_WORLD);
			}
			// MPI Broadcast new N particles from root
			MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);

			/* 
				Update velocity
			*/
			// first send to slave
			for (unsigned int r_i=1; r_i<size; r_i++){
				num_i = num_sent++;
				MPI_Send(&num_i, 1, MPI_INT, MPI_ANY_SOURCE, TAG_UPDATE_VELOCITY, MPI_COMM_WORLD);
			}
			for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
				// receive myparticles[indx] to root
				MPI_Recv(&particles[t_i], 1, MPI_PARTICLE, MPI_ANY_SOURCE, TAG_UPDATE_VELOCITY, MPI_COMM_WORLD, &status);
				// send indx particle to slave
				if (num_sent < N_PARTICLES){
					num_i = num_sent++;
					MPI_Send(&num_i, 1, MPI_INT, MPI_ANY_SOURCE, TAG_UPDATE_VELOCITY, MPI_COMM_WORLD);
				} else {
					MPI_Send(&num_i, 1, MPI_INT, t_i, TAG_DONE, MPI_COMM_WORLD);
				}
			}
			// MPI Broadcast new N particles from root
			MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);

			// save file in some step time
		}
	} else { /// Slave
		Particle my_particle;
		int tag, my_i, p_i;
		float v_half[2], a_0[2], a_1[2]; // to store data in x, y
		float dr[2], abs_dr[2];
		do {
			MPI_Recv(&my_i, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			tag = status.MPI_TAG;
			printf("Rank %d got tag %d", rank, tag);
			if (tag == TAG_DONE)
				break;
			my_particle = particles[my_i];
			/// update position particle and send to master
			if (tag == TAG_UPDATE_POSITION){
				// compute net a_0
				for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
					dr[0] = particles[p_i].x - my_particle.x >= 0.0f;
					dr[1] = particles[p_i].y - my_particle.y >= 0.0f;
					a_0[0] = particles[p_i].m*dr[0]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
					a_0[1] = particles[p_i].m*dr[1]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
				}
				// 1) step v_half
				v_half[0] = my_particle.vx + a_0[0]*STEP_TIME/2.0f;
				v_half[1] = my_particle.vy + a_0[1]*STEP_TIME/2.0f;
				// 2) update new position x_1
				my_particle.x = my_particle.x + v_half[0]*STEP_TIME;
				my_particle.y = my_particle.y + v_half[1]*STEP_TIME;
				// send to update new position and get again 
				MPI_Send(&my_particle, 1, MPI_PARTICLE, 0, TAG_UPDATE_POSITION, MPI_COMM_WORLD);				
			}
			/// update volocity
			if (tag == TAG_UPDATE_VELOCITY){
				// compute a_1
				for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
					dr[0] = particles[p_i].x - my_particle.x >= 0.0f;
					dr[1] = particles[p_i].y - my_particle.y >= 0.0f;
					a_1[0] = particles[p_i].m*dr[0]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
					a_1[1] = particles[p_i].m*dr[1]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
				}
				// 3) update new velocity v_1
				my_particle.vx = v_half[0] + a_1[0]*STEP_TIME/2.0f;
				my_particle.vy = v_half[1] + a_1[1]*STEP_TIME/2.0f;
				// send to update new velocity and get again
				MPI_Send(&my_particle, 1, MPI_PARTICLE, 0, TAG_UPDATE_VELOCITY, MPI_COMM_WORLD);
			}
			/// end update process
		} while(tag != TAG_DONE);
	}

	MPI_Finalize();
}