#ifndef MAIN
#define MAIN


#define WIDTH 1024
#define HEIGHT 1024
// #define FILE_MONO_IMG "monoturtle.dat"
#define FILE_MONO_IMG "resizedImg/monoturtle_large.dat"

// #define ITER_TIMES 10

int *vec_hist;
int *img;

void readImg(int *_img);
void writeFile(int *_vec_hist);

#endif