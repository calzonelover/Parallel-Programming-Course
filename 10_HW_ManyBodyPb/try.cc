#include <iostream>
#include <cstddef>
#include <mpi.h>

int main(int argc, char** argv){
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct simple {int i; double d[3];};
    simple v;
    constexpr std::size_t num_members = 2;
    int lengths[num_members] = {1, 3};
    MPI_Aint offsets[num_members] = {
        offsetof(simple, i),
        offsetof(simple, d),
    };
    MPI_Datatype types[num_members] = {MPI_INT, MPI_DOUBLE};
    MPI_Datatype simple_type;
    MPI_Type_struct(num_members, lengths, offsets, types, &simple_type);

    MPI_Type_commit(&simple_type);
    if (rank == 0){
        v.i = 3;
        v.d[0] = 1; v.d[1] = 3; v.d[2] = 909;
        // MPI_Send(&v, 1, simple_type, 1, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(&v, 1, simple_type, 0, MPI_COMM_WORLD);


    MPI_Status s;
    if (rank == 1){
        // MPI_Recv(&v, 1, simple_type, 0, 0, MPI_COMM_WORLD, &s);
        printf("i=%d d_0=%lf d_1=%lf d_2=%lf \n", v.i, v.d[0], v.d[1], v.d[2]);
    }


    MPI_Finalize();
    return 0;
}