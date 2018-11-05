#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>


#define XMAX 1.0
#define YMAX 1.0
#define NX 201
#define NY 201
#define V 0.1
#define NT 1000
#define A 1000.0
#define DT 0.035

#define TAG_STEP_WAVE 2
#define TAG_UPDATE_WAVE 3
#define TAG_DONE 4


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

	// variables
	float **wave2d_u0, **wave2d_u1, **wave2d_u2;
	wave2d_u0 = malloc(sizeof(float*)*NY);
	wave2d_u1 = malloc(sizeof(float*)*NY);
	wave2d_u2 = malloc(sizeof(float*)*NY);
	for (unsigned int iy=0; iy<NY; iy++){
		wave2d_u0[iy] = malloc(sizeof(float)*NX);
		wave2d_u1[iy] = malloc(sizeof(float)*NX);
		wave2d_u2[iy] = malloc(sizeof(float)*NX);
	}
	// initialize 
	float dx = XMAX/(NX-1);
	float dy = YMAX/(NY-1);
	if ( rank == 0 ){
		for (unsigned int iy=0; iy<NY; iy++){
			float y_now = iy+dy - 0.5f*YMAX;
			for (unsigned int ix=0; ix<NX; ix++){
				int indx = ix + iy*NX;
				float x_now = ix*DX - 0.5f*XMAX;
				wave2d_u1[indx] = wave2d_u0[indx] = exp(-A*(pow(x_now,2)+pow(y_now,2)));
			}
		}
	}
	for (unsigned int iy=0; iy<NY; iy++){
		MPI_Bcast(&wave2d_u0[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&wave2d_u1[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&wave2d_u2[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}
	// MPI_Bcast(&wave2d, NX*NY, MPI_WAVE, 0, MPI_COMM_WORLD);
	// loop over time
	/// Master
	if ( rank == 0){
		int num_sent;
		int slave_i;
		// loop over time
		for (unsigned int t_i=0; t_i < NT; t_i++){
			num_sent = 0;
			// step
			for (unsigned int ix=0; ix<NX; ix++){
				MPI_Send(&wave2d_u2, NX, MPI_FLOAT, r_i)
			}
			// update 
			for (unsigned int ix=0; ix<NX; ix++){

			}
		}
		for (unsigned int r_i=1; r_i<size; r_i++){
			MPI_Send(??, 1, r_i, TAG_DONE, MPI_COMM_WORLD);
		}
	} else {
	/// Slave
		int tag, my_i;
		do{
			// step
			if (TAG_STEP_WAVE){
				for (unsigned int ix=0; ix<NX; ix++){

				}
			} else if (){
			// update
				for (unsigned int ix=0; ix<NX; ix++){

				}
			}
		} while ( tag != TAG_DONE );
	}
}








