__kernel void fillHist(__global int *_img, __global int *_vec_hist){
	int idx = get_global_id(0);
	int pix_num = _img[idx];
	atomic_inc(vec_hist[pix_num]);
};