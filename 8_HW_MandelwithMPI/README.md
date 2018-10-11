# HW 6 OpenMP 
# Mr. Patomporn Payoungkhamdee SCPY6138171

## Numerical integration
* Compute integral from any given function y(x) by using small summation over region of graph in the given range

### Method
* Integrate from X_I to X_F which dx = (X_I - X_F)/N
* <a href="https://www.codecogs.com/eqnedit.php?latex=S&space;=&space;\sum_{x_i=1}^{n}&space;f_i(x_i)*dx" target="_blank"><img src="https://latex.codecogs.com/svg.latex?S&space;=&space;\sum_{x_i=1}^{n}&space;f_i(x_i)*dx" title="S = \sum_{x_i=1}^{n} f_i(x_i)*dx" /></a>
* Notice that x_i is the middle of the small rectangle in the summation

## Mandelbrot
Compute Mandelbrot set by dividing into sub-grid for 
compute and feeding back to combine into the global map

### Convention
* Suppose we have 512x512 grid cell and parallelly compute 128x128 sub-grid => prefer to use 4*4 = 16 processes with one waiter processes
* Consider range (-2, -2) to (2, 2)
* Set max_iteration as 10,000 even it still not diverge
* Shade of map is showing how each point converge when diverge mark as black

### Method
* Initialize non-blocking receiver all of the sub-grid process
* For non-zero rank, Init non-blocking sender compute for sending to waiter
* Visualize the result

### Result