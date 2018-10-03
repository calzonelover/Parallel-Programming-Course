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

  /*
   * Platform Query
   */

  // get CL_PLATFORM_PROFILE
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_PROFILE, 0, NULL, &size);
  char *profile = (char*) malloc(size*sizeof(char));
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_PROFILE, size, profile, NULL);
  printf("CL_PLATFORM_PROFILE = %s\n", profile);

  // get CL_PLATFORM_VERSION
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_VERSION, 0, NULL, &size);
  char *version = (char*) malloc(size*sizeof(char));
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_VERSION, size, version, NULL);
  printf("CL_PLATFORM_VERSION = %s\n", version);

  // get CL_PLATFORM_NAME
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, 0, NULL, &size);
  char *name = (char*) malloc(size*sizeof(char));
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, size, name, NULL);
  printf("CL_PLATFORM_NAME = %s\n", name);

  // get CL_PLATFORM_VENDOR
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_VENDOR, 0, NULL, &size);
  char *vendor = (char*) malloc(size*sizeof(char));
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_VENDOR, size, vendor, NULL);
  printf("CL_PLATFORM_VENDOR = %s\n", vendor);

  // get CL_PLATFORM_EXTENSIONS
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_EXTENSIONS, 0, NULL, &size);
  char *extensions = (char*) malloc(size*sizeof(char));
  status = clGetPlatformInfo(platforms[0], CL_PLATFORM_EXTENSIONS, size, extensions, NULL);
  printf("CL_PLATFORM_EXTENSIONS = %s\n", extensions);

  return 0;
}
