#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAG_DONE 0
#define TAG_INPROGRESS 1

#define W 1024
#define MAX_ITER 10000

typedef struct Complex
{
  float x;
  float y;
} Complex;


int iterMandel(Complex _c, float _max_width);
void writeFile(int **_map);

int main(int argc, char** argv) {
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numsent = 0;
    int pb_size = W;

    float x_i = atof(argv[1]);
    float x_f = atof(argv[2]);
    float y_i = atof(argv[3]);
    float y_f = atof(argv[4]);
    float max_width = x_f - x_i + y_f - y_i;

    float dx = (x_f - x_i) / W;
    float dy = (y_f - y_i) / W;

    if (rank == 0) {
        int **map, j;
        map = malloc(sizeof(int*)*pb_size);
        for (unsigned int i = 0; i < W; i++)
            map[i] = malloc(sizeof(int)*W); 
        // First send
        for (unsigned int i=1; i<size; i++) {
            j = numsent++;
            MPI_Send(&j, 1, MPI_INT, i, TAG_INPROGRESS, MPI_COMM_WORLD);
        }
        // loop until finish
        for (unsigned int i=0; i<pb_size; i++) {
            MPI_Recv(map[i], W, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int slave = status.MPI_SOURCE;
            // printf("Master receive from slave %d\n",  slave);
            if (numsent < pb_size) {
                j = numsent++;
                MPI_Send(&j, 1, MPI_INT, slave, TAG_INPROGRESS, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(&j, 1, MPI_INT, slave, TAG_DONE, MPI_COMM_WORLD);
            }
        }
        printf("Finish Work! \n");
        writeFile(map);
    }
    else {
        int g_i, d_i, d_j, pb_i;
        int tag, *slice_map;
        Complex c_local;
        slice_map = malloc(sizeof(int)*W);
        do {
            MPI_Recv(&pb_i, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            tag = status.MPI_TAG;
            // printf("Worker Slave %d: tag %d\n", rank, tag);
            if (tag == TAG_DONE)
                break;
            for (unsigned int i=0; i<W; i++){
                g_i = W*pb_i + i;
                d_i = g_i % W;
                d_j = (g_i % W > 0) ? (g_i - g_i%W)/W : g_i/W ;
                c_local.x = dx*d_i + x_i;
                c_local.y = dy*d_j + y_i;
                slice_map[i] = iterMandel(c_local, max_width);
            }
            MPI_Send(slice_map, W, MPI_INT, 0, rank, MPI_COMM_WORLD);
        } while (tag == TAG_INPROGRESS);
    }
  MPI_Finalize();
  return 0;
}

int iterMandel(Complex _c, float _max_width){
    int _out, _k=0;
    Complex _z, _z_prev;
    float _length_sq;
    _z.x = _z.y = 0.0f;
    do{
        _z_prev.x = _z.x;
        _z_prev.y = _z.y;
        _z.x = _z_prev.x*_z_prev.x - _z_prev.y*_z_prev.y + _c.x;
        _z.y = 2.0*_z_prev.x*_z_prev.y + _c.y;
        _length_sq = _z.x*_z.x + _z.y*_z.y;
        _k++;
    } while(_length_sq < _max_width && _k < MAX_ITER);
    if (_k == MAX_ITER){
        _out = 0;
    } else {
        _out = _k % 16;
    }
    return _out;
} 

void writeFile(int **_map){
    FILE *file;
    file = fopen("3_mandel_result.ppm", "w");
    fprintf(file, "P2 %d %d 16\n", W, W);
    for (unsigned int _i=0; _i<W; _i++){
        for (unsigned int _j=0; _j<W; _j++){
            fprintf(file, "%d \t", _map[_i][_j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    fclose(file);
}