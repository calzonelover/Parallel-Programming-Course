#ifndef WAVE2D
#define WAVE2D

#define xmax 1.0
#define ymax 1.0
#define nx 201
#define ny 201
#define v 0.1
#define nt 1000
#define a 1000.0
#define dt 0.035

#define MAX_ITER 1

size_t size;
clock_t start, stop;

float *u0, *u1, *u2;
float C, C2;

#endif
