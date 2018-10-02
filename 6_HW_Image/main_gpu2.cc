#define __CL_ENABLE_EXCEPTIONS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// #define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL/cl.hpp>
#include "main.h"

#define MAX_SOURCE_SIZE (0x100000)
#define BLOCK_SIZE 32


int main(){
	size_t size_img = WIDTH*HEIGHT*sizeof(int);
	size_t size_hist = 256*sizeof(int);
	img = (int*)malloc(size_img);
	vec_hist = (int*)malloc(size_hist);
	for (unsigned int i=0; i<256;i++) {vec_hist[i]=0;}
	/// OpenCL stuff

	cl::NDRange global(WIDTH*HEIGHT);
	cl::NDRange local(BLOCK_SIZE);

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
	cl::CommandQueue queue;

	cl::Buffer buf_img, buf_vec_hist;
	/// Program ///
	readImg(img);
	for (unsigned int i=0; i<256;i++) {vec_hist[i]=0;}
	// 1) setup
	cl::Platform::get(&platforms);
	platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
	cl::Context context(devices);
	// create cmd queue
	queue = cl::CommandQueue(context, devices[0]);
	// 2) construct compilation
	// load source
	std::ifstream sourceFile("kernel_fill_hist.cl");
	std::string sourceCode(
		std::istreambuf_iterator <char> (sourceFile),
		(std::istreambuf_iterator <char> ())
	);
	cl::Program::Sources source(
		1,
		std::make_pair(sourceCode.c_str(),sourceCode.length()+1)
	);
	// end load source
	cl::Program program = cl::Program(context, source);
	try{
		program.build(devices);
	} catch (cl::Error error){
		std::string log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
		std::cout << log << std::endl;
	}
	exit(0);
	cl::Kernel fillHistKernel(program, "fillHist");
	// 3) create memory buffer
	buf_img = cl::Buffer(context, CL_MEM_READ_ONLY, size_img);
	buf_vec_hist = cl::Buffer(context, CL_MEM_READ_WRITE, size_hist);
	// 4) copy host -> device
	queue.enqueueWriteBuffer(buf_img, CL_TRUE, 0, size_img, img);
	queue.enqueueWriteBuffer(buf_vec_hist, CL_TRUE, 0, size_hist, vec_hist);
	// 5) set kernel arguments
	fillHistKernel.setArg(0, buf_img);
	fillHistKernel.setArg(1, buf_vec_hist);
	// 6) execute
	queue.enqueueNDRangeKernel(fillHistKernel, cl::NullRange, global, local);
	// 7) cpy device -> host
	queue.enqueueReadBuffer(buf_vec_hist, CL_TRUE, 0, size_hist, vec_hist);
	// 8) wait until finish
	cl::finish();

	writeFile(vec_hist);
	// unalloc
	/*
	cl::releaseKernel(kernel);
	cl::releaseProgram(program);
	cl::releaseCommandQueue(cmdQueue);
	cl::releaseMemObject(buf_img);
	cl::releaseMemObject(buf_vec_hist);
	cl::releaseContext(context);*/
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
