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
	* Integrate from x_0 to x_1 which dx = (x_1 - x_0) / N
	* $S = sum_{x_i=1}^{n} f_i(x_i)*dx$