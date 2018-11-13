#include "wave_cudaMpi.h"

#include <cuda.h>
#include <cuda_runtime.h>


__global__ void kernel_stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2){
    int ix = threadIdx.x + blockIdx.x*blockDim.x;
    int iy = threadIdx.y + blockIdx.y*blockDim.y;
    int global_iy = NY/2*_rank + iy;
    int indx = ix+iy*NX;
    if (ix > 0 && global_iy > 0 && ix < NX -1 && global_iy < NY - 1){
        if (_rank == 0 && global_iy == NY/2 -1){
            _wave2d_u2[indx] = (2.0f-4.0f*_C2)*_wave2d_u1[indx] - _wave2d_u0[indx]
                        + _C2*(_wave2d_u1[(ix+1)+iy*nX]+_wave2d_u1[(ix-1)+iy*NX]
                        + _my_recv_halo[ix]+_wave2d_u1[ix+(iy-1)*NX]);
        } else if (_rank == 1 && global_iy == NY/2){
            _wave2d_u2[indx] = (2.0f-4.0f*_C2)*_wave2d_u1[indx] - _wave2d_u0[indx]
                        + _C2*(_wave2d_u1[(ix+1)+iy*nX]+_wave2d_u1[(ix-1)+iy*NX]
                        + _wave2d_u1[ix+(iy+1)*NX]+_my_recv_halo[ix]);
        } else {
            _wave2d_u2[indx] = (2.0f-4.0f*_C2)*_wave2d_u1[indx] - _wave2d_u0[indx]
                        + _C2*(_wave2d_u1[(ix+1)+iy*nX]+_wave2d_u1[(ix-1)+iy*NX]
                        + _wave2d_u1[ix+(iy+1)*NX]+_wave2d_u1[ix+(iy-1)*NX]);
        }
    }
}

__global__ void kernel_updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank){
    int ix = threadIdx.x + blockIdx.x*blockDim.x;
    int iy = threadIdx.y + blockIdx.y*blockDim.y;
    if (ix > 0 && iy > 0 && ix < nx-1 && iy < ny-1){
        int indx = ix+iy*nx;
        _wave2d_u0[indx] = _wave2d_u1[indx];
        _wave2d_u1[indx] = _wave2d_u2[indx];
    }
}

extern "C" void alloc_var_device(float *d_var, size_t size){
    cudaMalloc((void **)&d_var, size);
}

extern "C" void inject_var_to_device(float *d_var, float *h_var, size_t size){
    cudaMalloc((void **)&d_var, size);
    cudaMemcpy(d_var, h_var, size, cudaMemcpyHostToDevice);
}

extern "C" void load_var_to_host(float *h_var, float *d_var, size_t size){
    cudaMemcpy(h_var, d_var, size, cudaMemcpyDeviceToHost);
}

extern "C" void load_var_to_device(float *d_var, float *h_var, size_t size){
    cudaMemcpy(d_var, h_var, size, cudaMemcpyHostToDevice);
}

extern "C" void stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2){
    dim3 B(32, 32);
    dim3 G(NX/32+1, NY/32+1);
    kernel_stepWave<<<G,B>>>(_wave2d_u0, _wave2d_u1, _wave2d_u2, my_recv_halo, _rank, _C2);
}

extern "C" void updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank){
    dim3 B(32, 32);
    dim3 G(NX/32+1, NY/32+1);
    kernel_stepWave<<<G,B>>>(_wave2d_u0, _wave2d_u1, _wave2d_u2, _rank);
}