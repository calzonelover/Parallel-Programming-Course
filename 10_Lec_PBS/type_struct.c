#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main (int argc, char* argv[]){
  float a, b;
  int rank, size, n;
  MPI_Status status;
  MPI_Request request;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  if (rank == 0) {
    a = 1.0f;
    b = 2.0f;
    n = 3;
  }

  int block[3];
  MPI_Aint displacement[3];
  MPI_Datatype type[3];
  MPI_Aint start, stop;
  block[0] = block[1] = block[2] = 1;
  type[0] = MPI_FLOAT;
  type[1] = MPI_FLOAT;
  type[2] = MPI_INT;
  displacement[0] = 0;
  MPI_Get_address(&a, &start);
  MPI_Get_address(&b, &stop);
  displacement[1] = stop - start;
  MPI_Get_address(&n, &stop);
  displacement[2] = stop - start;
  MPI_Datatype CB_TYPE;
  MPI_Type_create_struct(3, block, displacement, type, &CB_TYPE);
  MPI_Type_commit(&CB_TYPE);
  MPI_Bcast(&a, 1, CB_TYPE, 0, MPI_COMM_WORLD);
  if (rank == 1)
    printf("a = %f, b = %f, n = %d\n", a, b, n);

  MPI_Finalize();
  return 0;
}