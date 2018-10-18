#include <stdio.h>
#include <mpi.h>

#define TAG_DONE 0
#define TAG_INPROGRESS 1

int main(int argc, char** argv) {
  int n = 5, i, j, numsent = 0;
  int rank, size;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    // First send
    for (i=1; i<size; i++) {
      j = numsent++;
      MPI_Send(&j, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    }
    for (i=0; i<n; i++) {
      MPI_Recv(&j, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      int slave = status.MPI_SOURCE;
      printf("Master receive %d from slave %d\n", j, slave);
      if (numsent < n) {
        j = numsent++;
        MPI_Send(&j, 1, MPI_INT, slave, TAG_INPROGRESS, MPI_COMM_WORLD);
      }
      else {
        MPI_Send(&j, 1, MPI_INT, slave, TAG_DONE, MPI_COMM_WORLD);
      }
    }

  }
  else {
    int tag;
    do {
      MPI_Recv(&j, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      tag = status.MPI_TAG;
      printf("Slave %d: receive %d, tag %d\n", rank, j, tag);
      if (tag == TAG_DONE) break;
      MPI_Send(&j, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    } while (tag == TAG_INPROGRESS);
  }
  MPI_Finalize();
  return 0;
}