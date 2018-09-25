CC=g++
NVCC=nvcc

all: build_cpu build_gpu run clean

build_cpu:
	$(CC) main.cc -o main_cpu
build_gpu:
	$(nvcc) main_gpu_1.cu -o main_gpu_1

clean:
    rm -rf test *.o
