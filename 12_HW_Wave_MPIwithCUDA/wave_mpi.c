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
#define NT 100
#define A 1000.0
#define DT 0.035

#define TAG_STEP_WAVE 2
#define TAG_UPDATE_WAVE 3
#define TAG_SYNC_HALO 4
#define TAG_DONE 5


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
    float *wave2d_u0, *wave2d_u1, *wave2d_u2;
	wave2d_u0 = malloc(sizeof(float)*(NY/2)*NX);
	wave2d_u1 = malloc(sizeof(float)*(NY/2)*NX);
	wave2d_u2 = malloc(sizeof(float)*(NY/2)*NX);    
    float *my_recv_halo, *my_send_halo;
    my_recv_halo = malloc(sizeof(float)*NX);
    my_send_halo = malloc(sizeof(float)*NX);

    // init value
    init_variables(wave2d_u0, wave2d_u1, dx, rank);
    // Loop over time step
    for (unsigned int t_i=0; t_i < NT; t_i++){
        sync_halo(wave2d_u1, my_send_halo, my_recv_halo, rank, request, status);
        stepWave(wave2d_u0, wave2d_u1, wave2d_u2, my_recv_halo, rank, C2);
        // for (unsigned int i=0; i<NX; i++) printf("wave2d_u2[%d] = %f \n", i, wave2d_u2[i]);
        updateWave(wave2d_u0, wave2d_u1, wave2d_u2, rank);
    }
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

void stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2){
    unsigned int _global_y_i;
    for (unsigned int y_i = 0; y_i<NY/2; y_i++){
        _global_y_i = NY/2*_rank + y_i;
        // if (t_i == 2 && rank == 0) printf("rank %d y_i %d \n", rank, y_i);
        for (unsigned int x_i=0; x_i<NX; x_i++){
            if (x_i > 0 && _global_y_i > 0 && x_i < NX-1 && _global_y_i < NY-1){
                if ( _rank == 0 && y_i == NY/2-1){
                _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
                                        + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
                                        + _my_recv_halo[x_i]+_wave2d_u1[(y_i-1)*NX+x_i]);
                } else if ( _rank == 1 && y_i == 0){
                _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
                                        + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
                                        + _wave2d_u1[(y_i+1)*NX+x_i]+_my_recv_halo[x_i]);
                } else {
                _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
                                        + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
                                        + _wave2d_u1[(y_i+1)*NX+x_i]+_wave2d_u1[(y_i-1)*NX+x_i]);
                }
            }
        }
    }
}

void updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank){
    unsigned int _global_y_i;
    for (unsigned int y_i = 0; y_i<NY/2; y_i++){
        _global_y_i = NY/2*_rank + y_i;
        for (unsigned int x_i=0; x_i<NX; x_i++){
            if (x_i > 0 && _global_y_i > 0 && x_i < NX-1 && _global_y_i < NY-1){
                _wave2d_u0[y_i*NX+x_i] = _wave2d_u1[y_i*NX+x_i];
                _wave2d_u1[y_i*NX+x_i] = _wave2d_u2[y_i*NX+x_i];
            }
        }
    }
}