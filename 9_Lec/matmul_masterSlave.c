#include <stdio.h>
#include <mpi.h>


#define SIZE_VEC 9

void master(int _size_pcs);
void slave(int _rank, int _size_pcs);


int main(int argc, char *argv[]){
	int size_processes, rank, number;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0){
		master(size_processes);
	} else {
		slave(rank, size_processes);
	}
	MPI_Finalize();
	return 0;
}



void master(int _size_pcs){
	int *arr;
	MPI_Status stat_s[_size_pcs-1], stat_r[_size_pcs-1];

	arr = (int*)malloc(sizeof(int)*SIZE_VEC);
	for (unsigned int i=0; i<SIZE_VEC; i++)
		arr[i]=i+1;
	
	int arr_n = 0;
	while (1){		
		for (unsigned int p=1; p<_size_pcs-1; p++){
			if (!stat_s[p].MPI_ERROR){
				MPI_Send(&arr[arr_n], 1, MPI_INT, p, MPI_COMM_WORLD, &stat_s[p]);
				arr_n++;
				printf("Already send index %d\n", arr_n);
				if (arr_n == SIZE_VEC)
					printf("Processs done !\n");
					break;
			}
		}
	}
}

void slave(int _rank, int _size_pcs){
	int sub_arr;
	MPI_Status stat_s, stat_r;
	MPI_Recv(&sub_arr, 1, MPI_INT, 0, _rank, MPI_COMM_WORLD, &stat_r);
	printf("Process rank %d Receive value %d \n", _rank, sub_arr);
}