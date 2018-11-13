#include "wave_cudaMPI.h"

#include <cuda.h>
#include <cuda_runtime.h>


extern "C" void inject_var_to_device(float *h_var, float *d_var, size_t size){
    cudaMalloc((void **)&d_var, size);
    cudaMemcpy(d_var, h_var, size, cudaMemcpyHostToDevice);
}

extern "C" void load_var_to_host(float *wave2d_u0, float *wave2d_u1, float *wave2d_u2, float *d_wave2d_u0, float *d_wave2d_u1, float *d_wave2d_u2, size_t my_size){
    cudaMemcpy(wave2d_u0, d_wave2d_u0, my_size, cudaMemcpyDeviceToHost);
    cudaMemcpy(wave2d_u1, d_wave2d_u1, my_size, cudaMemcpyDeviceToHost);
    cudaMemcpy(wave2d_u2, d_wave2d_u2, my_size, cudaMemcpyDeviceToHost);
}

extern "C" void load_var_to_device(float *h_var, float *d_var, size_t size){
    cudaMemcpy(d_var, h_var, size, cudaMemcpyHostToDevice);
}

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

extern "C" void stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2){
    
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

extern "C" void updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank){

}


// void stepWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, float *_my_recv_halo, int _rank, float _C2){
//     unsigned int _global_y_i;
//     for (unsigned int y_i = 0; y_i<NY/2; y_i++){
//         _global_y_i = NY/2*_rank + y_i;
//         // if (t_i == 2 && rank == 0) printf("rank %d y_i %d \n", rank, y_i);
//         for (unsigned int x_i=0; x_i<NX; x_i++){
//             if (x_i > 0 && _global_y_i > 0 && x_i < NX-1 && _global_y_i < NY-1){
//                 if (_rank == 0 && _global_y_i == NY/2-1){
//                 _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
//                                         + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
//                                         + _my_recv_halo[x_i]+_wave2d_u1[(y_i-1)*NX+x_i]);
//                 } else if (_rank == 1 && _global_y_i == NY/2){
//                 _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
//                                         + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
//                                         + _wave2d_u1[(y_i+1)*NX+x_i]+_my_recv_halo[x_i]);
//                 } else {
//                 _wave2d_u2[y_i*NX+x_i] = (2.0f-4.0f*_C2)*_wave2d_u1[y_i*NX+x_i] - _wave2d_u0[y_i*NX+x_i]
//                                         + _C2*(_wave2d_u1[y_i*NX+(x_i+1)]+_wave2d_u1[y_i*NX+(x_i-1)]
//                                         + _wave2d_u1[(y_i+1)*NX+x_i]+_wave2d_u1[(y_i-1)*NX+x_i]);
//                 }
//             }
//         }
//     }
// }

// void updateWave(float *_wave2d_u0, float *_wave2d_u1, float *_wave2d_u2, int _rank){
//     unsigned int _global_y_i;
//     for (unsigned int y_i = 0; y_i<NY/2; y_i++){
//         _global_y_i = NY/2*_rank + y_i;
//         for (unsigned int x_i=0; x_i<NX; x_i++){
//             if (x_i > 0 && _global_y_i > 0 && x_i < NX-1 && _global_y_i < NY-1){
//                 _wave2d_u0[y_i*NX+x_i] = _wave2d_u1[y_i*NX+x_i];
//                 _wave2d_u1[y_i*NX+x_i] = _wave2d_u2[y_i*NX+x_i];
//             }
//         }
//     }
// }