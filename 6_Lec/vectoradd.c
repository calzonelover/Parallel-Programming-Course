#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


const char* programSource = 
"__kernel void vecadd(__global int *A, __global int *B, __global *C){ \n"
"  int idx = get_global_id(0); \n"
"  C[idx] = A[idx]+B[idx]; \n"
"}";

int main() {
  const int elements = 2048;
  size_t size = sizeof(int)*elements;
  int *A = (int*)malloc(size);
  int *B = (int*)malloc(size);
  int *C = (int*)malloc(size);
  int i;
  for (i=0; i<elements; i++) {
    A[i] = i;
    B[i] = i;
  }
  cl_int status;
  cl_platform_id platform;
  status = clGetPlatformIDs(1, &platform, NULL);
  cl_device_id device;
  status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
  cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
  cl_command_queue cmdQueue = clCreateCommandQueue(context, device, 0, &status);
  // alloc mem
  cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, &status);
  cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, &status);
  cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, NULL, &status);
  // cpy from host to device
  status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0, size, A, 0, NULL, NULL);
  status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_FALSE, 0, size, B, 0, NULL, NULL);
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&programSource, NULL, &status);
  status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
  cl_kernel kernel = clCreateKernel(program, "vecadd", &status);
  status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
  status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
  size_t grid[1], block[1];
  grid[0] = elements;
  block[0] = 256;
  status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, grid, block, 0, NULL, NULL);
  status = clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, size, C, 0, NULL, NULL);
  for (i=0; i<10; i++)
    printf("C[%d] = %d\n", i, C[i]);

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(cmdQueue);
  clReleaseMemObject(bufA);
  clReleaseMemObject(bufB);
  clReleaseMemObject(bufC);
  clReleaseContext(context);
  free(A);
  free(B);
  free(C);
  return 0;
}
