compile:
	mpicxx -c test1.cpp -o test1.o -lm
	nvcc -c kernel_test1.cu -o kernel_test1.o
	mpicxx test1.o kernel_test1.o -lcudart -lm -L/usr/local/cuda-9.0/lib64/ -o program_test1
run:
	# mpiexec -n 2 ./program_test1
	mpirun -x LD_LIBRARY_PATH -n 2 -pernode -machinefile machinefile ./program_test1
clean:
	rm -f *.o
	rm program_test1