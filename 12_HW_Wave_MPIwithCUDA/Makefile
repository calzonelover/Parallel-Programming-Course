compile:
	mpicc -c wave_cudaMpi.c -o wave_cudaMpi.o -lm
	nvcc -c wave_cuda_kernel.cu -o wave_cuda_kernel.o
	mpicc wave_cudaMpi.o wave_cuda_kernel.o -o program_cudaMpi
run:
	mpiexec -n 2 ./program_cudaMpi
clean:
	rm -f *.o
	rm program_cudaMpi