#include <stdio.h>

#include "integral.h"


int main(int argc, char *argv[]){
	float result;
	result = integrate(X_I, X_F);
	printf("Result = %f\n", result);
	return 0;
}


float func(float _x_in){
	return 2.0f;
}

float integrate(float _x_i, float _x_f){
	float dx = (float) (_x_f - _x_i) / N;
	float sum = 0.0f;
	float _x_now;
	for (unsigned int i=0; i<N; i++){
		_x_now = _x_i*dx;
		sum += func(_x_now)*dx;
	}
	return sum;
}





















