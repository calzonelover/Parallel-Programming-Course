#include "mpi.h"
#include <iostream>

#include "test1.h"

int main(int argc, char *argv[])
{
/* It's important to put this call at the begining of the program, after variable declarations. */
    MPI::Init(argc, argv);

    int rank = MPI::COMM_WORLD.Get_rank();
    int size = MPI::COMM_WORLD.Get_size();

    std::cout << rank << std::endl;
    // ==== Call function 'call_me_maybe' from CUDA file multiply.cu: ==========
    call_me_maybe();
    MPI::Finalize();
    return 0;
}