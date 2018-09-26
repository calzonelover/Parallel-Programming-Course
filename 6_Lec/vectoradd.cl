__kernel void vecadd(__global int *A, __global int *B, __global *C){
  int idx = get_global_id(0);
  C[idx] = A[idx]+B[idx];
};
