#define WIDTH 1024
#define HEIGHT 1024
__kernel void fillHist(__global int *_img, __global int *_vec_hist){
	__local int _local_vec_hist[256];
	int local_id = get_local_id(0);
	int global_id = get_global_id(0);
	for (unsigned int i = local_id; i<256; i++){
		_local_vec_hist[i] = 0;
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	for (unsigned int i = global_id; i<WIDTH*HEIGHT; i+=get_global_size(0)){
		atomic_inc(&_local_vec_hist[i]);
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	for (unsigned int i= local_id; i< 256; i+= get_local_size(0)){
		atomic_add(_vec_hist[i], &_local_vec_hist[i]);
	}
}
