#include <stdio.h>
#include <math.h>
__global__ void
MatAdd(const float *A, const float *B, float *C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    int j = blockDim.y * blockIdx.y + threadIdx.y;
    if (i < N && j < N)
    {
        int indx = i + j*N;
        C[indx] = A[indx] + B[indx];
    }
}

/**
 * Host main routine
 */
int
main(void)
{
    // Print the vector length to be used, and compute its size
    int N = 100;
    size_t size = N*N*sizeof(float);
    printf("[Matrix size of %dx%d elements]\n", N, N);
    // allocate memory
    float *h_A = (float *)malloc(size);
    float *h_B = (float *)malloc(size);
    float *h_C = (float *)malloc(size);

    // Initialize the host input vectors
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N ; ++j){
            h_A[i+j*N] = 1;//rand()/(float)RAND_MAX;
            h_B[i+j*N] = 1; rand()/(float)RAND_MAX;
        }
    }
    // ALLOCATE DEVICE MEMORY
    float *d_A = NULL;
    cudaMalloc((void **)&d_A, size);
    float *d_B = NULL;
    cudaMalloc((void **)&d_B, size);
    float *d_C = NULL;
    cudaMalloc((void **)&d_C, size);

    // Copy the host input vectors A and B in host memory to the device input vectors in
    // device memory
    printf("Copy input data from the host memory to the CUDA device\n");
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    /// Beware no more than that 
    // Launch the Vector Add CUDA Kernel
    dim3 B(32,32);// B.x=32, B.y=32, B.z=1
    dim3 G(N/32 +1, N/32+1);
    printf("CUDA kernel launch with %dx%dx%d blocks of %dx%dx%d threads\n", B.x, B.y, B.z, G.x, G.y, G.z);
    MatAdd<<<G, B>>>(d_A, d_B, d_C, N);

    // Copy the device result vector in device memory to the host result vector
    // in host memory.
    printf("Copy output data from the CUDA device to the host memory\n");
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // verify the result
    float sum = 0;
    for (unsigned int i=0; i < N ; ++i){
        for (unsigned int j=0; j < N; ++j){
            sum += h_C[i+j*N];
        }
    }
    if (fabs(sum-N*N*2) < 1.0e-5){
        printf("Test PASSED\n");
    }else{
        printf("Test Don't pass: you fucked up!\n");
    }

    // Free device global memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);

    printf("Done\n");
    return 0;
}

