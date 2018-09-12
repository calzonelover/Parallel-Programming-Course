#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "randomwalk.h"

#define IS_WRITE_WALKER 1
#define IS_WRITE_RCUDE 1

int main(){
	FILE *file = fopen("position_walker.txt","w");
	FILE *file_rcube = fopen("rcube.txt","w");
	x = malloc(WALKER*sizeof(float));
	y = malloc(WALKER*sizeof(float));
	srand(time(NULL));
	initPosition(x, y);
	for (unsigned int i=0; i<STEP; i++){
		walk(x, y);
		float mrc = meanRcube(x,y);
		if (IS_WRITE_WALKER){
			for (unsigned int w=0; w<WALKER; w++){
				fprintf(file, "%10.4f \t %10.4f\n", x[w], y[w]);
			}
		}
		if (IS_WRITE_RCUDE)
			fprintf(file_rcube, "%f\n", mrc);
		//printf("<r^2> = %f \n",mrc);
	}
	fclose(file);
	fclose(file_rcube);
	return 0;
}




void initPosition(float *_x, float *_y){
	for (unsigned int i=0; i<WALKER; i++){
		_x[i] = 0.0f;
		_y[i] = 0.0f;
	}
}

void walk(float *_x, float *_y){
	for (unsigned int i=0; i<WALKER; i++){
		float rand_x = (float)rand()/ (float)RAND_MAX;
		float rand_y = (float)rand()/ (float)RAND_MAX;
		_x[i] += rand_x*(X_MAX-X_MIN) + X_MIN;
		_y[i] += rand_y*(X_MAX-X_MIN) + X_MIN;
	}
}

float meanRcube(float *_x, float *_y){
	float _rc = 0.0f, _mrc = 0.0f;
	for (unsigned int i=0; i<WALKER;i++){
		_rc += _x[i]*_x[i]+_y[i]*_y[i];
	}
	_mrc = (float)_rc/WALKER;
	return _mrc;
}


