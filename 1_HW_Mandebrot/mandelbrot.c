#include <stdio.h>
#include <time.h>

#include "mandelbrot.h"

int main(int argc, char const *argv[])
{

  int data[n_x][n_y];
  float dx = (x_f - x_i)/n_x;
  float dy = (y_f - y_i)/n_y;
  int k;
  Complex z_prev;
  
  start = clock();
  for (unsigned int i=0; i<n_x; i++){
    for (unsigned int j=0; j<n_y; j++){
      z.x = z.y = 0.0;
      k = 0;
      c.x = dx*i + x_i;
      c.y = dy*j + y_i;
      do {
        z_prev.x = z.x;
        z_prev.y = z.y;
        z.x = z_prev.x*z_prev.x - z_prev.y*z_prev.y + c.x;
        z.y = 2.0*z_prev.x*z_prev.y + c.y;
        length_sq = z.x*z.x + z.y*z.y;
        k++;
      } while (length_sq < (x_f - x_i + y_f - y_i) && k < max_iter );
      if (k == max_iter)
        k = 0;
      else
        k = k%16;
      data[i][j] = k;
    }
  }
  end = clock();
  printf("CPU time = %lf\n", (double) (end - start)/ CLOCKS_PER_SEC );

  file = fopen("mandel.ppm", "w");
  fprintf(file, "P2 %d %d 16\n", n_x, n_y);
  for (unsigned int i=0; i<n_x; i++){
    for (unsigned int j=0; j<n_y; j++){
      fprintf(file, "%d \t", data[i][j]);
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
  fclose(file);

}