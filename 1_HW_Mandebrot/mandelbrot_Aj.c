#include <stdio.h>
#include <time.h>

typedef struct
{
  float x;  // real part
  float y;  // imag part
} Complex;

int main (int argc, char **argv)
{
  int max_iter = 10000;
  int nx = 600;
  int ny = 600;
  int i, j, k;
  int data[nx][ny];
  Complex z, c;
  float lengthsq, temp;
  FILE *file;
  clock_t start, end;
  
  start = clock();
  for(i=0; i < nx; i++) {
    for(j=0; j < ny; j++) {
      z.x = z.y = 0.0;
      c.x = (4.0*j - 2.0*nx)/nx;
      c.y = (4.0*i - 2.0*ny)/ny;
      k = 0;
      do {
        temp = z.x*z.x - z.y*z.y + c.x;
        z.y = 2.0*z.x*z.y + c.y;
        z.x = temp;
        lengthsq = z.x*z.x+z.y*z.y;
        k++;
      } while (lengthsq < 4.0 && k < max_iter);
      if (k == max_iter)
        k = 0;
      else
          k = k%16;
      data[i][j] = k;
    }
  }
  end = clock();
  double cpu_time = (double) (end-start) / CLOCKS_PER_SEC;
  printf("CPU time = %lf\n", cpu_time);

  // Save the data into .ppm file.
  file = fopen("mandel.ppm","w");
  fprintf(file, "P2 %d %d 16\n", nx, ny);
  for(i=0; i < nx; i++) {
    for(j=0; j < ny; j++) {
      fprintf(file, "%d \t", data[i][j]);
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
  fclose(file);
  
  return 0;
}
