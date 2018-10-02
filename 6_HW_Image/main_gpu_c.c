#include <stdlib.h>
#include <stdio.h>

// #define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL/cl.h>
#include "main.h"

#define MAX_SOURCE_SIZE (0x100000)
#define BLOCK_SIZE 4

void loadKernelSource(char *_programSource);

int main(){
	size_t size_img = WIDTH*HEIGHT*sizeof(int);
	size_t size_hist = 256*sizeof(int);
	img = (int*)malloc(size_img);
	vec_hist = (int*)malloc(size_hist);
	for (unsigned int i=0; i<256;i++) {vec_hist[i]=0;}
	/// OpenCL stuff
	char *programSource = 
	"__kernel void fillHist(__global int *_img, __global int *_vec_hist){\n"
	"	int idx = get_global_id(0);\n"
	"	int pix_num = _img[idx]\n;"
	"	_vec_hist[pix_num]++;\n"
	"}";
	// char* programSource;
	// programSource = (char *)malloc(MAX_SOURCE_SIZE);
	// loadKernelSource(programSource);

	size_t grid, block;
	grid = WIDTH*HEIGHT;
	block = BLOCK_SIZE;

	cl_int status;
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue cmdQueue;

	cl_program program;
	cl_kernel kernel;
	cl_mem buf_img, buf_vec_hist;

	/// Program ///
	readImg(img);
	// 1) setup
	status = clGetPlatformIDs(1, &platform, NULL);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
	cmdQueue = clCreateCommandQueue(context, device, 0, &status);
	// 2) construct compilation
	program = clCreateProgramWithSource(context, 1, (const char**)&programSource, NULL, &status);
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);	
	kernel = clCreateKernel(program, "fillHist", &status);
	// 3) create memory buffer
	buf_img = clCreateBuffer(context, CL_MEM_READ_ONLY, size_img, NULL, &status);
	buf_vec_hist = clCreateBuffer(context, CL_MEM_READ_WRITE, size_hist, NULL, &status);
	// 4) copy host -> device
	status = clEnqueueWriteBuffer(cmdQueue, buf_img, CL_FALSE, 0, size_img, img, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, buf_vec_hist, CL_FALSE, 0, size_hist, vec_hist, 0, NULL, NULL);
	// 5) set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf_img);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf_vec_hist);
	// 6) execute
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, &grid, &block, 0, NULL, NULL);
	// 7) cpy device -> host
	status = clEnqueueReadBuffer(cmdQueue, buf_vec_hist, CL_TRUE, 0, size_hist, vec_hist, 0, NULL, NULL);
	// 8) wait until finish
	clFinish(cmdQueue);

	writeFile(vec_hist);
	// unalloc
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(buf_img);
	clReleaseMemObject(buf_vec_hist);
	clReleaseContext(context);
	free(img);
	free(vec_hist);
	return 0;
}


void readImg(int *_img){
	std::ifstream f_img(FILE_MONO_IMG);
	for (unsigned int i=0; i<WIDTH; i++){
		for (unsigned int j=0; j<HEIGHT; j++){
			int x;
			f_img >> x;
			if (f_img.eof()) break;
			_img[i+j*WIDTH] = x;
		}
	}
	f_img.close();
}

void writeFile(int *_vec_hist ){
	std::ofstream out_hist;
	out_hist.open("histogram_gpu.dat");
	for (unsigned int d=0; d < 256; d++){
		out_hist << _vec_hist[d] << std::endl;
	}
	out_hist.close();
}

void loadKernelSource(char *_programSource){
	FILE *fp;
	const char filename[] = "./kernel_fill_hist.cl";
	size_t source_size;
	fp = fopen(filename, "r");
	source_size = fread(_programSource, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
}