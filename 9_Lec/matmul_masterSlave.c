#include <stdio.h>
#include <mpi.h>
int main(int argc, char** argv) {
  int n = 10, i, j, numsent = 0;
  int rank, size;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    for (i=0; i<size-1; i++) {
      j = numsent++;
      MPI_Send(&j, 1, MPI_INT, i+1, 1, MPI_COMM_WORLD);
    }
    for (i=0; i<n; i++) {
      MPI_Recv(&j, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      int slave = status.MPI_SOURCE;
      printf("receive %d from rank %d\n", j, slave);
      if (numsent < n) {
        j = numsent++;
        MPI_Send(&j, 1, MPI_INT, slave, 1, MPI_COMM_WORLD);
      }
      else {
        MPI_Send(&j, 1, MPI_INT, slave, 0, MPI_COMM_WORLD);
      }
    }
  }
  else {
    int tag;
    do {
      MPI_Recv(&j, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      tag = status.MPI_TAG;
      printf("Rank %d: receive %d, tag %d\n", rank, j, tag);
      if (tag == 0) break;
      MPI_Send(&j, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    } while (tag == 1);
  }
  MPI_Finalize();
  return 0;
}