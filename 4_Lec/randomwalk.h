#ifndef RANDOMWALK
#define RANDOMWALK

#define W 200
#define H 200
#define STEP 10000
#define WALKER 100
#define X_MIN -1.0
#define X_MAX 1.0

float *x,*y;

void walk(float *_x, float *_y);
void initPosition(float *_x, float *_y);
float meanRcube(float *_x, float *_y);

#endif
