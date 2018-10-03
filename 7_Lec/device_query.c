#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
int main() {
  cl_int status;
  size_t size;

  // get the number of platforms
  cl_uint numPlatforms = 0;
  status = clGetPlatformIDs(0, NULL, &numPlatforms);
  printf("number of available platforms = %d\n", numPlatforms);

  // allocate enough space for each platform
  cl_platform_id *platforms = NULL;
  platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

  // fill in the platforms
  status = clGetPlatformIDs(numPlatforms, platforms, NULL);

  cl_uint numDevices;
  cl_device_id devices[1];

  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
  printf("Number of GPU devices = %d\n", numDevices);

  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &devices[0], NULL);

  /*
   * Device query
   */
  cl_uint maxComputeUnits;
  status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint),
    &maxComputeUnits, &size);
  printf("Number of cores (SMs) = %d\n", maxComputeUnits);

  return 0;
}
