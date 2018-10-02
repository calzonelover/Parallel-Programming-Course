#define WIDTH 1024
#define HEIGHT 1024

__kernel void fillHist(__global int *_img, __global int *_vec_hist){
    int indx = get_global_id(0);
    int pixel = _img[indx];
    atom_inc(&_vec_hist[pixel]);
}
