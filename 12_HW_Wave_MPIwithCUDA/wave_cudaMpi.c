#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>

#include "wave_cudaMpi.h"


void writeFile(float *_map);
void init_variables(float *wave2d_u0,float *wave2d_u1, float _dx, int _rank);
void sync_halo(float *_wave2d_u1, float *_my_send_halo, float *_my_recv_halo, int _rank, MPI_Request _request[], MPI_Status _status[]);
void stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2);
void updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank);

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
    global_map = malloc(sizeof(float)*NY*NX);

    size_t my_size = sizeof(float)*NY/2*NX;
    size_t size_vec = sizeof(float)*NX;
    float *wave2d_u0, *wave2d_u1, *wave2d_u2;
	wave2d_u0 = malloc(my_size);
	wave2d_u1 = malloc(my_size);
	wave2d_u2 = malloc(my_size);    
    float *my_recv_halo, *my_send_halo;
    my_recv_halo = malloc(size_vec);
    my_send_halo = malloc(size_vec);

    float *d_wave2d_u0, *d_wave2d_u1, *d_wave2d_u2;
    float *d_my_recv_halo;
    // init value
    init_variables(wave2d_u0, wave2d_u1, dx, rank);
    inject_var_to_device(wave2d_u0, d_wave2d_u0, my_size);
    inject_var_to_device(wave2d_u1, d_wave2d_u1, my_size);
    inject_var_to_device(wave2d_u2, d_wave2d_u2, my_size);
    inject_var_to_device(my_recv_halo, d_my_recv_halo, size_vec);
    // Loop over time step
    for (unsigned int t_i=0; t_i < NT; t_i++){
        // sync halo
        load_var_to_host(wave2d_u1, d_wave2d_u1, my_size);
        MPI_Barrier(MPI_COMM_WORLD);
        sync_halo(wave2d_u1, my_send_halo, my_recv_halo, rank, request, status);
        load_var_to_device(d_my_recv_halo, my_recv_halo, size_vec);
        // step and update
        stepWave(wave2d_u0, wave2d_u1, wave2d_u2, my_recv_halo, rank, C2);
        updateWave(wave2d_u0, wave2d_u1, wave2d_u2, rank);
    }
    load_var_to_host(wave2d_u0, d_wave2d_u0, my_size);
    // collect and write file
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(wave2d_u0, NX*NY/2, MPI_FLOAT, global_map, NX*NY/2, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0)
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

void init_variables(float *_wave2d_u0,float *_wave2d_u1, float _dx, int _rank){
    float x_now, y_now;
    for (unsigned int iy=0; iy<NY/2; iy++){
        y_now = (_rank == 0)? iy*_dx - 0.5f*YMAX : iy*_dx;
        for (unsigned int ix=0; ix<NX; ix++){
            x_now = ix*_dx - 0.5f*XMAX;
            _wave2d_u0[iy*NX+ix] = (float) exp(-A*(pow(x_now,2)+pow(y_now,2)));
            _wave2d_u1[iy*NX+ix] = _wave2d_u0[iy*NX+ix];
        }
    }
}

void sync_halo(float *_wave2d_u1, float *_my_send_halo, float *_my_recv_halo, int _rank, MPI_Request _request[], MPI_Status _status[]){
    for (unsigned int i_x=0; i_x<NX; i_x++){
        if (_rank == 0)
            _my_send_halo[i_x] = _wave2d_u1[(NY/2-1)*NX+i_x];
        else
            _my_send_halo[i_x] = _wave2d_u1[i_x];
    }
    if (_rank == 0){
        MPI_Isend(_my_send_halo, NX, MPI_FLOAT, 1, TAG_SYNC_HALO, MPI_COMM_WORLD, &_request[0]);   
        MPI_Irecv(_my_recv_halo, NX, MPI_FLOAT, 1, TAG_SYNC_HALO, MPI_COMM_WORLD, &_request[1]);                     
    } else {
        MPI_Isend(_my_send_halo, NX, MPI_FLOAT, 0, TAG_SYNC_HALO, MPI_COMM_WORLD, &_request[0]);
        MPI_Irecv(_my_recv_halo, NX, MPI_FLOAT, 0, TAG_SYNC_HALO, MPI_COMM_WORLD, &_request[1]);
    }
    MPI_Wait(&_request[1], &_status[1]);
}