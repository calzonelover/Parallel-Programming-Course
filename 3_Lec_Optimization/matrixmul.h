#ifndef MATMUL
#define MATMUL

#define WA 320
#define HA 320
#define WB 640
#define HB 320

size_t size;
clock_t start, stop;
float *A, *B, *C;

void initMatrix(float *_M, int _W, int _H);
void printMatrix(float *_M, int _W, int _H);

#endif
