#ifndef N_BODY
#define N_BODY

#define N_PARTICLES 10
#define NT 1000
#define PERIOD_SNAP 1
#define STEP_TIME 0.0001

#define X_I -1.0
#define X_F 1.0
#define Y_I -1.0
#define Y_F 1.0

#define X_MAX 0.6
#define V_MAX 1.5
#define MASS 5.0

#define TAG_DONE 1
#define TAG_UPDATE_POSITION 2
#define TAG_UPDATE_VELOCITY 3
#define TAG_BCAST 4

typedef struct{
	float x, y;
	float vx, vy;
	float v_half_x, v_half_y;
	float m;
} Particle;


void snapShot(Particle _particles[], int _t_i);

#endif