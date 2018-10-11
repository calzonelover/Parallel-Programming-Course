# Lecture 4

* Each processes has a unique rank
* Tips: "Dead lock" happen when crossing send and receiving process
* Important ope:
	* Broadcast: "one" to "all"
	* Gather: collect all small buf to one
	* Scatter vice versa of gather
	* Reduce: Operate all small buff from all processes to one

## Example
* Integral function 
	* Integrate from X_I to X_F which dx = (X_I - X_F)/N
	* <a href="https://www.codecogs.com/eqnedit.php?latex=S&space;=&space;\sum_{x_i=1}^{n}&space;f_i(x_i)*dx" target="_blank"><img src="https://latex.codecogs.com/svg.latex?S&space;=&space;\sum_{x_i=1}^{n}&space;f_i(x_i)*dx" title="S = \sum_{x_i=1}^{n} f_i(x_i)*dx" /></a>
	* Notice that x_i is the middle of the small rectangle in the summation
## Great ref
* https://www.idi.ntnu.no/~elster/tdt4200-f09/gropp-mpi-tutorial.pdf
* https://htor.inf.ethz.ch/teaching/mpi_tutorials/ppopp13/2013-02-24-ppopp-mpi-basic.pdf