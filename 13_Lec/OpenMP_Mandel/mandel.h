#ifndef MANDELBROT
#define MANDELBROT


#define NX 2048
#define NY 2048
#define MAX_ITER 10000

#define XI -2.0
#define XF 2.0
#define YI -2.0
#define YF 2.0

typedef struct Complex
{
  float x;  // real part
  float y;  // imag part
} Complex;

double start_t, stop_t;
int compute_mandel(int _ix, int _iy, float _dx, float _dy);

#endif