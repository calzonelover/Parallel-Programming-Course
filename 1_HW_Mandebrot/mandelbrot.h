#ifndef MANDELBROT
#define MANDELBROT

#define n_x 1200
#define n_y 1200
#define max_iter 10000

typedef struct Complex
{
  float x;  // real part
  float y;  // imag part
} Complex;

//int n_x = 600;
//int n_y = 600;

float x_i = -2.0;
float y_i = -2.0;
float x_f = 2.0;
float y_f = 2.0;

Complex z, c;
float length_sq;
FILE *file;
clock_t start, end;


#endif
