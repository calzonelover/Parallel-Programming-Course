#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>

#define XMAX 1.0
#define YMAX 1.0
#define NX 256
#define NY 256
#define V 0.1
#define NT 10
#define A 1000.0
#define DT 0.035

#define TAG_STEP_WAVE 2
#define TAG_UPDATE_WAVE 3
#define TAG_SYNC_HALO 4
#define TAG_DONE 5


void writeFile(float *_map);

int main(int argc, char** argv){
	int rank, size;
    MPI_Request request[2];
	MPI_Status status[2];
	double start_t, stop_t;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0){
		printf("number of processes = %d\n", size);
	}
	printf("Init process rank %d\n", rank);

    // variable
	float dx = XMAX/(NX-1);
	float C = V*DT/dx;
	float C2 = C*C;

    float *global_map;
    global_map = malloc(sizeof(float*)*NY*NX);
    float *wave2d_u0, *wave2d_u1, *wave2d_u2;
	wave2d_u0 = malloc(sizeof(float*)*NY/2*NX);
	wave2d_u1 = malloc(sizeof(float*)*NY/2*NX);
	wave2d_u2 = malloc(sizeof(float*)*NY/2*NX);    
    float *my_recv_halo, *my_send_halo;
    my_recv_halo = malloc(sizeof(float)*NX);
    my_send_halo = malloc(sizeof(float)*NX);

    unsigned int y_i;
    unsigned int max_y_i = (rank == 0)? NY/2: NY;
    // init value
    float x_now, y_now;
    for (unsigned int iy=0; iy<NY/2; iy++){
        y_now = (rank == 0)? iy*dx - 0.5f*YMAX : iy*dx;
        for (unsigned int ix=0; ix<NX; ix++){
            x_now = ix*dx - 0.5f*XMAX;
            wave2d_u0[iy*NX+ix] = (float) exp(-A*(pow(x_now,2)+pow(y_now,2)));
            wave2d_u1[iy*NX+ix] = wave2d_u0[iy*NX+ix];
        }
    }
    // Loop over time step
    for (unsigned int t_i=0; t_i < NT; t_i++){
        // assign send halo
        for (unsigned int i_x=0; i_x<NX; i_x++){
            if (rank == 0)
                my_send_halo[i_x] = wave2d_u1[(NY/2-1)*NX+i_x];
            else
                my_send_halo[i_x] = wave2d_u1[(NY/2)*NX+i_x];
        }
        // sync halo
        if (rank == 0){
            MPI_Isend(my_send_halo, NX, MPI_FLOAT, 1, TAG_SYNC_HALO, MPI_COMM_WORLD, &request[0]);   
            MPI_Irecv(my_recv_halo, NX, MPI_FLOAT, 1, TAG_SYNC_HALO, MPI_COMM_WORLD, &request[1]);                     
        } else {
            MPI_Isend(my_send_halo, NX, MPI_FLOAT, 0, TAG_SYNC_HALO, MPI_COMM_WORLD, &request[0]);
            MPI_Irecv(my_recv_halo, NX, MPI_FLOAT, 0, TAG_SYNC_HALO, MPI_COMM_WORLD, &request[1]);
        }
        MPI_Waitall(2, request, status);
        // step our 2D map
        for (unsigned int y_i = (rank == 0)? 0: NY/2; y_i<max_y_i; y_i++){
            // if (t_i == 2 && rank == 0) printf("rank %d y_i %d \n", rank, y_i);
            for (unsigned int x_i=0; x_i<NX; x_i++){
                if (x_i > 0 && y_i > 0 && x_i < NX-1 && y_i < NY-1){
                    if (rank == 0 && y_i == NY/2-1){
                    wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*C2)*wave2d_u1[y_i*NX+x_i] - wave2d_u0[y_i*NX+x_i]
		        					      + C2*(wave2d_u1[y_i*NX+(x_i+1)]+wave2d_u1[y_i*NX+(x_i-1)]
        		      					  + my_recv_halo[x_i]+wave2d_u1[(y_i-1)*NX+x_i]);                       
                    } else if (rank == 1 && y_i == NY/2){
                    wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*C2)*wave2d_u1[y_i*NX+x_i] - wave2d_u0[y_i*NX+x_i]
		        					      + C2*(wave2d_u1[y_i*NX+(x_i+1)]+wave2d_u1[y_i*NX+(x_i-1)]
        		      					  + wave2d_u1[(y_i+1)*NX+x_i]+my_recv_halo[x_i]);
                    } else {
                    wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*C2)*wave2d_u1[y_i*NX+x_i] - wave2d_u0[y_i*NX+x_i]
		        					      + C2*(wave2d_u1[y_i*NX+(x_i+1)]+wave2d_u1[y_i*NX+(x_i-1)]
        		      					  + wave2d_u1[(y_i+1)*NX+x_i]+wave2d_u1[(y_i-1)*NX+x_i]);
                    }
                }
            }
        }
        // update our 2D map
        for (unsigned int y_i = (rank == 0)? 0: NY/2; y_i<max_y_i; y_i++){
            for (unsigned int x_i=0; x_i<NX; x_i++){
                if (x_i > 0 && y_i > 0 && x_i < NX-1 && y_i < NY-1){
                    wave2d_u0[y_i*NX+x_i] = wave2d_u1[y_i*NX+x_i];
                    wave2d_u1[y_i*NX+x_i] = wave2d_u2[y_i*NX+x_i];
                }
            }
        }
    }
    // root collect and write file

    MPI_Gather(wave2d_u0, NX*NY/2, MPI_FLOAT, global_map, NX*NY/2, MPI_FLOAT, 0, MPI_COMM_WORLD);
    writeFile(global_map);
    // done
    MPI_Finalize();
    return 0;
}


void writeFile(float *_map){
	FILE *file;
	file = fopen("wave2d_map.dat", "w");
	for (unsigned int _i=0; _i<NX; _i++){
		for (unsigned int _j=0; _j<NY; _j++){
			fprintf(file, "%f \t", _map[_i+_j*NX]);
            // printf("%f ",_map[_i+_j*NX]);
		}
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
	fclose(file);
}