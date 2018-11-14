# for halo
gohalo:
	mpicc wave_mpi.c -o wave_mpi.x -lm -std=c99
	mpiexec -n 2 wave_mpi.x
# for parallel
compile:
	mpicc -c wave_cudaMpi.c -o wave_cudaMpi.o -lm
	nvcc -c wave_cuda_kernel.cu -o wave_cuda_kernel.o
	mpicc wave_cudaMpi.o wave_cuda_kernel.o -lcudart -lm -L/usr/local/cuda-9.0/lib64/ -o program_cudaMpi
run_local:
	mpiexec -n 2 ./program_cudaMpi
run:
	mpirun -x LD_LIBRARY_PATH -n 2 -pernode -machinefile machinefile ./program_cudaMpi
clean:
	rm -f *.o
	rm program_cudaMpi