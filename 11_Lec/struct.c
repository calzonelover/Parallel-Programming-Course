#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float x,y;
	float vx, vy;
	float m;
} Particle;

/*
int main(int argc, char** argv){
	Particle p;
	p.x = 1.0f; p.y = 2.0f; p.vx = 3.0f; p.vy = 4.0f;
	float *a = (float *)&p;
	for (unsigned int i=0; i<5; i++){
		printf("a[%d] = %f\n", i, a[i]);
	}
	return 0;
}
*/

int main(int argc, char** argv){
	Particle *p;
	float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	p = (Particle *) a;
	printf("p[0].x = %f\n", p[0].x);
	printf("p[0].y = %f\n", p[0].y);
	printf("p[0].vx = %f\n", p[0].vx);
	printf("p[0].vy = %f\n", p[0].vy);
	return 0;
}














