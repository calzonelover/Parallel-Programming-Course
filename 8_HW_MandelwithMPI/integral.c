#include <stdio.h>

#include "integral.h"



int main(int argc, char *argv[]){
	float result;
	float dx = (float) (X_F - X_I) / N;
	result = integrate(X_I, X_F, dx);
	printf("Result = %f\n", result);
	return 0;
}


float func(float _x_in){
	return 2.0f;
}

float integrate(float _x_i, float _x_f, float _dx){
	float _sum = 0.0f;
	float _x_now = _x_i + _dx/2.0f;
	while(_x_now < _x_f){
		_sum += func(_x_now)*_dx;
		_x_now += _dx;
	}
	return _sum;
}





















