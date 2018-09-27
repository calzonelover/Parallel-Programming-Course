#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "main.h"


void fillHistogram(int *_img, int *_vec_hist);
void writeFile(int *_vec_hist );

int main(){
	img = (int*)malloc(WIDTH*HEIGHT*sizeof(int));
	vec_hist = (int*)malloc(256*sizeof(int));

	readImg(img);
	fillHistogram(img, vec_hist);
	writeFile(vec_hist);
	// std::cout << vec_hist[120] << std::endl;

	return 0;
}

void readImg(int *_img){
	std::ifstream f_img(FILE_MONO_IMG);
	for (unsigned int i=0; i<WIDTH; i++){
		for (unsigned int j=0; j<HEIGHT; j++){
			int x;
			f_img >> x;
			if (f_img.eof()) break;
			img[i+j*WIDTH] = x;
		}
	}
	f_img.close();
}

void fillHistogram(int *_img, int *_vec_hist){
	for (unsigned int d=0; d < 256; d++){
		_vec_hist[d] = 0;
	}
	for (unsigned int d=0; d < WIDTH*HEIGHT; d++){
		int indx = _img[d];
		_vec_hist[indx]++;
	}
}

void writeFile(int *_vec_hist ){
	std::ofstream out_hist;
	out_hist.open("histogram_cpu.dat");
	for (unsigned int d=0; d < 256; d++){
		out_hist << _vec_hist[d] << std::endl;
	}
	out_hist.close();
}