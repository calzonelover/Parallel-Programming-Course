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
#define NT 10
#define A 1000.0
#define DT 0.035

#define TAG_STEP_WAVE 2
#define TAG_UPDATE_WAVE 3
#define TAG_BCAST 4
#define TAG_DONE 5


void writeFile(float **_map);

int main(int argc, char** argv){
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
	float dx = XMAX/(NX-1);
	float C = V*DT/dx;
	float C2 = C*C;

	float **wave2d_u0, **wave2d_u1, **wave2d_u2;
	wave2d_u0 = malloc(sizeof(float*)*NY);
	wave2d_u1 = malloc(sizeof(float*)*NY);
	wave2d_u2 = malloc(sizeof(float*)*NY);
	for (unsigned int iy=0; iy<NY; iy++){
		wave2d_u0[iy] = malloc(sizeof(float)*NX);
		wave2d_u1[iy] = malloc(sizeof(float)*NX);
		wave2d_u2[iy] = (float*)malloc(sizeof(float)*NX);
	}
	// initialize
	float x_now, y_now;
	if ( rank == 0 ){
		for (unsigned int iy=0; iy<NY; iy++){
			y_now = iy*dx - 0.5f*YMAX;
			for (unsigned int ix=0; ix<NX; ix++){
				x_now = ix*dx - 0.5f*XMAX;
				wave2d_u0[iy][ix] = (float) exp(-A*(pow(x_now,2)+pow(y_now,2)));
				wave2d_u1[iy][ix] = wave2d_u0[iy][ix];
				// if ((float)exp(-A*(pow(x_now,2.0f)+pow(y_now,2.0f))) > 0.00001f){
				// 	printf("value %f \n", (float)exp(-A*(pow(x_now,2.0f)+pow(y_now,2.0f))));
				// 	printf("wave 2d u0 = %f u1 = %f \n", wave2d_u1[iy][ix], wave2d_u1[iy][ix] );
				// }
			}
		}
	}
	for (unsigned int iy=0; iy<NY; iy++){
		MPI_Bcast(wave2d_u0[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
		MPI_Bcast(wave2d_u1[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}
	// loop over time
	/// Master
	if ( rank == 0){
		int num_sent;
		int slave_i;
		// loop over time
		for (unsigned int t_i=0; t_i < NT; t_i++){
			num_sent = 0;
			// step wave
			for (unsigned int r_i=1; r_i < size; r_i++){
				MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_STEP_WAVE, MPI_COMM_WORLD);
				num_sent++;
			}
			for (unsigned int iy=0; iy<NY; iy++){
				MPI_Recv(wave2d_u2[iy], NX, MPI_FLOAT, MPI_ANY_SOURCE, TAG_STEP_WAVE, MPI_COMM_WORLD, &status);
				// for(unsigned int i=0; i<NX; i++){
				// 	printf("wave 2d u0 = %f u1 = %f \n", wave2d_u1[iy][i], wave2d_u1[iy][i] );
				// }
				slave_i = status.MPI_SOURCE;
				if (num_sent < NY){
					MPI_Send(&num_sent, 1, MPI_INT, slave_i, TAG_STEP_WAVE, MPI_COMM_WORLD);
					num_sent++;
				}
			}
			// update wave
			for (unsigned int iy=0; iy<NY; iy++){
				for (unsigned int ix=0; ix<NX; ix++){
					if (ix > 0 && iy > 0 && ix < NX -1 && iy < NY -1 ){
						wave2d_u0[iy][ix] = wave2d_u1[iy][ix];
						wave2d_u1[iy][ix] = wave2d_u2[iy][ix];
					} else {
						wave2d_u0[iy][ix] = 0.0f;
						wave2d_u1[iy][ix] = 0.0f;
					}
				}
			}
			for (unsigned int r_i=1; r_i<size; r_i++){
				MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_BCAST, MPI_COMM_WORLD);
			}
			for (unsigned int iy=0; iy<NY; iy++){
				MPI_Bcast(wave2d_u0[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
				MPI_Bcast(wave2d_u1[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
				MPI_Bcast(wave2d_u2[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
			}
		}
		// end loop time
		for (unsigned int r_i=1; r_i<size; r_i++){
			MPI_Send(&num_sent, 1, MPI_INT, r_i, TAG_DONE, MPI_COMM_WORLD);
		}
		// write file
		writeFile(wave2d_u2);
	} else {
	/// Slave
		int tag, my_iy;
		float *my_slice_u2;
		do{
			MPI_Recv(&my_iy, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			tag = status.MPI_TAG;
			// printf("rank %d receive tag %d with iy = %d \n", rank, tag, my_iy);
			// step
			if (tag == TAG_STEP_WAVE){
				for (unsigned int ix=0; ix<NX; ix++){
					if (ix > 0 && my_iy > 0 && ix < NX -1 && my_iy < NY -1 ){
						wave2d_u2[my_iy][ix] = (2.0f-4.0f*C2)*wave2d_u1[my_iy][ix] - wave2d_u0[my_iy][ix]
		        					      + C2*(wave2d_u1[my_iy][ix+1]+wave2d_u1[my_iy][ix-1]
        		      					  + wave2d_u1[my_iy+1][ix]+wave2d_u1[my_iy-1][ix]);
						// if (wave2d_u2[my_iy][ix] > 0.0001f)
							// printf("value u2 = %f \n", wave2d_u2[my_iy][ix]);
					}
				}
				MPI_Send(wave2d_u2[my_iy], NX, MPI_FLOAT, 0, TAG_STEP_WAVE, MPI_COMM_WORLD);
			} else if ( tag == TAG_BCAST ){
			// update
				for (unsigned int iy=0; iy<NY; iy++){
					MPI_Bcast(wave2d_u0[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
					MPI_Bcast(wave2d_u1[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
					MPI_Bcast(wave2d_u2[iy], NX, MPI_FLOAT, 0, MPI_COMM_WORLD);
				}
			}
		} while ( tag != TAG_DONE );
	}
	MPI_Finalize();
	return 0;
}


void writeFile(float **_map){
	FILE *file;
	file = fopen("wave2d_raw_map.dat", "w");
	for (unsigned int _i=0; _i<NX; _i++){
		for (unsigned int _j=0; _j<NY; _j++){
			fprintf(file, "%f \t", _map[_i][_j]);
		}
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
	fclose(file);
}





