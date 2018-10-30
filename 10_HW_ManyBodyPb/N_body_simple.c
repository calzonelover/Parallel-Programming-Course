#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>

#include "N_body.h"

void declareNewType(MPI_Datatype _new_dat_type);

void main(int argc, char** argv){
	int rank, size;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0){
		printf("number of processes = %d\n", size);
	}
	printf("Init process rank %d\n", rank);

	MPI_Datatype MPI_PARTICLE;
	MPI_Type_contiguous(7, MPI_FLOAT, &MPI_PARTICLE);
	MPI_Type_commit(&MPI_PARTICLE);
	// declareNewType(MPI_PARTICLE);

	// initialize N particles
	Particle particles[N_PARTICLES];
	Particle my_particle;

	if (rank == 0){
		Particle particles[N_PARTICLES];
		srand(time(NULL));
		for (unsigned int i = 0; i < N_PARTICLES; i++){
			particles[i].x = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)X_MAX;
			particles[i].y = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)X_MAX;
			particles[i].vx = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)V_MAX;
			particles[i].vy = (float) ((rand()/ (float)RAND_MAX - 0.5f)* 2.0f) * (float)V_MAX;
			particles[i].m = MASS;
			printf("p_i %d, x=%f, y=%f, vx=%f, vy=%f, mass=%f \n", i, particles[i].x, particles[i].y, particles[i].vx, particles[i].vy, particles[i].m);
		}
	}
	MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);
	// for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
	// 	printf("Time init Before Particle_%d x = %f y = %f vx=%f vy=%f \n", p_i, particles[p_i].x, particles[p_i].y, particles[p_i].vx, particles[p_i].vy);
	// }
	/// Master
	if (rank == 0){
		int num_sent;
		int slave_i;
		// Loop for step time
		for (unsigned int t_i=0; t_i < NT; t_i++){
			num_sent = 0;
			// first send to slave
			for (unsigned int r_i=1; r_i<size; r_i++){
				MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_UPDATE_POSITION, MPI_COMM_WORLD);
				num_sent++;
			}
			for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
				// receive myparticles[indx] to root
				MPI_Recv(&particles[p_i], 1, MPI_PARTICLE, MPI_ANY_SOURCE, TAG_UPDATE_POSITION, MPI_COMM_WORLD, &status);
				slave_i = status.MPI_SOURCE;
				// send indx particle to slave
				if (num_sent < N_PARTICLES){
					MPI_Send(&num_sent, 1, MPI_INT, slave_i, TAG_UPDATE_POSITION, MPI_COMM_WORLD);
					num_sent++;
				}
			}
			// MPI Broadcast new N particles from root
			for (unsigned int r_i=1; r_i<size; r_i++){
				MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_BCAST, MPI_COMM_WORLD);
			}
			MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);
			// save file in some step time
            // for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
	        // 	printf("Time %d Particle_%d x = %f y = %f vx=%f vy=%f \n", t_i, p_i, particles[p_i].x, particles[p_i].y, particles[p_i].vx, particles[p_i].vy);
            // }
			if (t_i % PERIOD_SNAP == 0)
				snapShot(particles, t_i);
		}
		// kill slave
		for (unsigned int r_i=1; r_i<size; r_i++){
				MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_DONE, MPI_COMM_WORLD);
		}

	} else { /// Slave
		int tag, my_i;
		float v_half[2], a_0[2] = {0,0}, a_1[2] = {0,0}; // to store data in x, y
		float dr[2], abs_dr[2];
		do {
			MPI_Recv(&my_i, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			tag = status.MPI_TAG;
			// printf("Rank %d got tag %d", rank, tag);
			if (tag == TAG_DONE)
				break;
			my_particle = particles[my_i];
			/// update volocity
			if (tag == TAG_UPDATE_POSITION){
				// compute a_1
				for (unsigned int p_i=0; p_i < N_PARTICLES; p_i++){
					if (p_i != my_i){
						dr[0] = particles[p_i].x - my_particle.x;
						dr[1] = particles[p_i].y - my_particle.y;
						a_1[0] += particles[p_i].m*dr[0]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
						a_1[1] += particles[p_i].m*dr[1]/pow( dr[0]*dr[0] + dr[1]*dr[1], 3.0f/2.0f);
					}
				}
                // printf("my_i %d a1_x = %f, a1_y = %f \n", my_i, a_1[0], a_1[1]);
				// 3) update new velocity v_1
				my_particle.vx += a_1[0]*STEP_TIME; a_1[0] = 0.0f;
				my_particle.vy += a_1[1]*STEP_TIME; a_1[1] = 0.0f;
                my_particle.x += my_particle.vx*STEP_TIME;
                my_particle.y += my_particle.vy*STEP_TIME;
				// printf("test in slave : x = %f y = %f \n", my_particle.vx*STEP_TIME, my_particle.vy*STEP_TIME);
				// send to update new velocity and get again
				MPI_Send(&my_particle, 1, MPI_PARTICLE, 0, TAG_UPDATE_POSITION, MPI_COMM_WORLD);
			}
			if (tag == TAG_BCAST){
				MPI_Bcast(&particles, N_PARTICLES, MPI_PARTICLE, 0, MPI_COMM_WORLD);
			}
			/// end update process
		} while(tag != TAG_DONE );
	}

	MPI_Finalize();
}

void snapShot(Particle _particles[], int _t_i){
	FILE *file;
	char file_name[200];
	sprintf(file_name, "result/Snapshot_t_%d.dat", _t_i);
	file = fopen(file_name, "w");
    for (unsigned int _p_i=0; _p_i<N_PARTICLES; _p_i++){
        fprintf(file, "%f %f\n", _particles[_p_i].x, _particles[_p_i].y);
    }
    fclose(file);
}

void declareNewType(MPI_Datatype _new_dat_type){
	int _N = 7; // from our dat type
	Particle my_par;
	int _block[_N];
	MPI_Aint _displacement[_N];
	MPI_Datatype _type[_N];
	MPI_Aint _start, _stop;
	for (unsigned int i=0; i<_N;i++){
		_block[i] = 1;
		_type[i] = MPI_FLOAT;
	}
	// get displacement
	_displacement[0] = 0;
	MPI_Get_address(&my_par.x, &_start);
	MPI_Get_address(&my_par.y, &_stop);
	_displacement[1] = _stop - _start;
	MPI_Get_address(&my_par.y, &_start);
	MPI_Get_address(&my_par.vx, &_stop);
	_displacement[2] = _stop - _start;
	MPI_Get_address(&my_par.vx, &_start);
	MPI_Get_address(&my_par.vy, &_stop);
	_displacement[3] = _stop - _start;
	MPI_Get_address(&my_par.vy, &_start);
	MPI_Get_address(&my_par.v_half_x, &_stop);
	_displacement[4] = _stop - _start;
	MPI_Get_address(&my_par.v_half_x, &_start);
	MPI_Get_address(&my_par.v_half_y, &_stop);
	_displacement[5] = _stop - _start;
	MPI_Get_address(&my_par.v_half_y, &_start);
	MPI_Get_address(&my_par.m, &_stop);
	_displacement[6] = _stop - _start;

	MPI_Type_create_struct(_N, _block, _displacement, _type, &_new_dat_type);
	MPI_Type_commit(&_new_dat_type);
}