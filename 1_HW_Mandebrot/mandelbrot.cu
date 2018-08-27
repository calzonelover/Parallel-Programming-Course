#include <stdio.h>
#include <time.h>

#include "mandelbrot.h"

__global__ void 
MandelFunc (float *Zx, float *Zy, int *K
, float dx, float dy
, float x_i, float y_i, float x_f, float y_f, int iter_num)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;
	int indx = i + j*n_y;
	if (i < n_x && j < n_y && K[indx] == 0 ){
		float Zx_prev = Zx[indx];
		float Zy_prev = Zy[indx];
		Zx[indx] = Zx_prev*Zx_prev - Zy_prev*Zy_prev + (dx*j+x_i);
		Zy[indx] = 2.0*Zx_prev*Zy_prev + (dy*i+y_i);
		if ( Zx[indx]*Zx[indx] +  Zy[indx]*Zy[indx] > (x_f - x_i + y_f - y_i))
			K[indx] =  iter_num%16;
	}
//	printf("length_sq = %f \n", Zx[indx]*Zx[indx] +  Zy[indx]*Zy[indx]);
}

int main(int argc, char const *argv[])
{
	start = clock();
	float dx = (x_f - x_i)/n_x;
	float dy = (y_f - y_i)/n_y;
	size_t size = n_x*n_x*sizeof(float);

	// dim of Block grid
	dim3 B(32, 32);
	dim3 G(n_x/32 +1 , n_y/32 +1);
	// allocate memory
	float *h_Zx = (float *)malloc(size);
	float *h_Zy = (float *)malloc(size);
	int *h_k = (int *)malloc(size);
	// allocate GPU memory
	float *d_Zx = NULL;
	float *d_Zy = NULL;
	int *d_k = NULL;
	cudaMalloc((void **)&d_Zx, size);
	cudaMalloc((void **)&d_Zy, size);
	cudaMalloc((void **)&d_k, size);


	// init value
	for (unsigned int i=0; i<n_x; i++){
    	for (unsigned int j=0; j<n_y; j++){
    		h_Zx[i+j*n_y] = h_Zy[i+j*n_y] = h_k[i+j*n_y] = 0;
    	}
    }
    // move memory to device
//    printf("Copy input data from the host memory to the CUDA device\n");
    cudaMemcpy(d_Zx, h_Zx, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_Zy, h_Zy, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_k, h_k, size, cudaMemcpyHostToDevice);

  //  printf("CUDA kernel launch with %dx%dx%d blocks of %dx%dx%d threads\n", B.x, B.y, B.z, G.x, G.y, G.z);

    for (unsigned int l=0; l < max_iter; l++){
    	MandelFunc<<<G, B>>>(d_Zx, d_Zy, d_k, dx, dy, x_i, y_i, x_f, y_f, l);
    }
    
    cudaMemcpy(h_k, d_k, size, cudaMemcpyDeviceToHost);

    file = fopen("mandel_gpu.ppm", "w");
	fprintf(file, "P2 %d %d 16\n", n_x, n_y);
	for (unsigned int i=0; i<n_x; i++){
		for (unsigned int j=0; j<n_y; j++){
	    		fprintf(file, "%d \t", h_k[i+j*n_y]);
		}
	    fprintf(file, "\n");
	}
	fprintf(file, "\n");
	fclose(file);

    // free device global memory
    cudaFree(d_Zx);
    cudaFree(d_Zy);
    cudaFree(d_k);
    // free host memory
    free(h_Zx);
    free(h_Zy);
    free(h_k);

    end = clock();
    printf("GPU time = %lf\n", (double) (end - start)/ CLOCKS_PER_SEC );

	return 0;
}
