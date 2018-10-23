#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define WIDTH 1024
#define HEIGHT 1024
#define MAX_ITER 10000

#define X_I -2.
#define X_F 2.
#define Y_I -2.
#define Y_F 2.

typedef struct Complex
{
  float x;
  float y;
} Complex;


int main(int argc, char *argv[]){
	clock_t start_t, stop_t; double cpu_time;
	int size_process, rank, number;
	size_t size_map;

	Complex z, z_prev, c;
	float dx = (float) (X_F-X_I)/WIDTH;
	float dy = (float) (Y_F-Y_I)/HEIGHT;
	int k;
	float length_sq;
	FILE *file;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
		printf("Number of comm_size (processes) = %d\n", size_process);

	// For now we ne need to use it
	MPI_Request request[size_process];
	MPI_Status status[size_process];

	// Alloc map memory
	size_map = WIDTH*HEIGHT*sizeof(int);
	int *map;
	if (rank == 0){
		map = (int *)malloc(size_map);
	}

	int sub_elements = WIDTH*HEIGHT/size_process;
	if (rank == 0)
		printf("sub_elements = %d\n", sub_elements);
	int sub_map[WIDTH*HEIGHT/size_process];

	if (rank == 0)
		start_t = clock();
	// Compute map
	for (unsigned int l_i=0;l_i < sub_elements; l_i++)
	// for (unsigned int i=sub_elements*rank;i < sub_elements*(rank+1); i++)
	{
		int g_i = l_i+sub_elements*rank;
		int d_i = g_i % WIDTH;
		int d_j = (g_i % WIDTH > 0) ? (g_i-g_i%WIDTH)/WIDTH : g_i/WIDTH ;
		z.x = z.y = 0.0f;
		k = 0;
		c.x = dx*d_j + X_I;
		c.y = dy*d_i + Y_I;
		do {
			z_prev.x = z.x;
			z_prev.y = z.y;
        	z.x = z_prev.x*z_prev.x - z_prev.y*z_prev.y + c.x;
	    	z.y = 2.0*z_prev.x*z_prev.y + c.y;
    		length_sq = z.x*z.x + z.y*z.y;
    		k++;
		} while (length_sq < (X_F-X_I + Y_F-Y_I) && k < MAX_ITER);
		if ( k == MAX_ITER ){
			sub_map[l_i] = 0;
		} else {
			sub_map[l_i] = k % 16;
		}
	}
	printf("Job done with rank = %d !! \n", rank);
	// Gather
	MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0){
    	stop_t = clock();
		cpu_time = (double) (stop_t-start_t) / CLOCKS_PER_SEC ;
		printf("Static load balancing CPU time %lf s\n", cpu_time);
    }
    // Collect all sub
	MPI_Gather(sub_map, sub_elements, MPI_INT, map, sub_elements, MPI_INT, 0, MPI_COMM_WORLD);
	// write file
	if (rank == 0){
		file = fopen("mandel_result.ppm", "w");
		fprintf(file, "P2 %d %d 16\n", WIDTH, HEIGHT);
		for (unsigned int i=0; i<WIDTH; i++){
			for (unsigned int j=0; j<HEIGHT; j++){
				fprintf(file, "%d \t", map[i+j*WIDTH]);
			}
			fprintf(file, "\n");
		}
		fprintf(file, "\n");
		fclose(file);
	}
	MPI_Finalize();
	return 0;
}