#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <time.h>
#include <math.h>

__global__ void reduce(int *g_idata, int *g_odata){
  extern __shared__ int sdata[];
  unsigned int tid = threadIdx.x; 
  unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
  sdata[tid] = g_idata[i];
  __syncthreads();
  
  for(unsigned int s = blockDim.x/2;s>0;s>>=1){
    if(tid<s){
      sdata[tid] += sdata[tid+s];
    }
    __syncthreads();
  }
  
  if(tid==0) g_odata[blockIdx.x] = sdata[0];
}

///////////////////////////////////////////////////////////////////////////

void init(int *A, int nA) {
  for (int i=0; i<nA; i++){
      A[i] = (int)rand() % 16;
  }
}

int c_summation(int *A, int nA) {
  int sum=A[0];
  for (int i=1; i<nA; i++) {
        sum += A[i];
  }
  return sum;
}

int main() {
  int order = 18;
  int nA = pow(2,order);
  printf("Vector size: %d\n", nA); 
  size_t sizeA = nA*sizeof(int);
  int c_sum,pr_sum;
  int *A,*S;
  A = (int*) malloc(sizeA); 
  S = (int*) malloc(sizeA); 
  
  srand(time(NULL));
  init(A, nA);
  
  int *dA =NULL;
  int *dS =NULL;
  cudaMalloc((void**)&dA,sizeA);
  cudaMalloc((void**)&dS,sizeA);
  cudaMemcpy(dA,A,sizeA,cudaMemcpyHostToDevice);
  //cudaMemcpy(dS,S,sizeA,cudaMemcpyHostToDevice);

  int B = 1024;
  int G = (nA+B-1)/B;
  int smemSize = B*sizeof(int);
  
  float kernel_time;
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start,0);
  //////////////////////////// Start time record
  reduce<<<G,B,smemSize>>>(dA,dS);
  // for (unsigned int stride=B; stride < 1 ; stride >>= 1){
  for(int problemsize=nA/B;problemsize > 1;problemsize/=B){
    reduce<<<G,B,smemSize>>>(dS,dS);
  }
  cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&kernel_time,start,stop);
  printf("GPU time = %lf ms\n", kernel_time);
  
  cudaMemcpy(S,dS,sizeA,cudaMemcpyDeviceToHost);
  pr_sum = S[0]; 
  
  clock_t begin, end;
  begin = clock();
  c_sum = c_summation(A,nA);
  end = clock();
  double cpu_time = (double) (end-begin)/ CLOCKS_PER_SEC *1000.0f;
  printf("CPU time = %lf ms\n", cpu_time);
  
  printf("Parallel reduction sum result is: %d \n",pr_sum);
  printf("c function sum result is: %d \n", c_sum);

  free(A);
  cudaFree(dA);
  
  return 0;
}
