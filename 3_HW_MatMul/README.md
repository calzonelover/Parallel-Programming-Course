# HW : Matrix Multiplication
## Matrix multiplication 
* For a given matrices A and B with dimension (WA, HA) (WB, HB) sequentially
* The product from multiplication C would has dimension (WB, HA)
* This relation can be written as  <br/>
	<img style="text-align: center;" src="https://latex.codecogs.com/gif.latex?C_{i,j}&space;=&space;\sum_{d}^{\text{WA}\equiv&space;\text{HB}}&space;A_{d,j}\cdot&space;B_{i,d}" title="C_{i,j} = \sum_{d}^{\text{WA}\equiv \text{HB}} A_{d,j}\cdot B_{i,d}" />
* Note that all matrix elements is all float number with dimension
	* (WA, HA) = (320, 320)
	* (WB, HB) = (640, 320)
	* Which cause (WC, HC) = (WB, HA)
* 10 times iteration of calculation for more precise time elapsed due to we use a small matrix size
* Notice that dimension matching of matrices for multiplication have to have WA = HB and it also already take into account for checking process before calculation in this code

## Methods
* Pure cutomize function (CPU)
* Customize kernel CUDA (GPU) for unshared memory and shared memory method for both of input matrices
* Libraly CUBLAS (GPU)

## Results
### Timing
* Customize C : 0.272511 s
* Customize CUDA kernel 
	* Ordinary Kernel : 0.003077 s
	* Shared memory of matrix A : 0.000184 s ?
	* Shared memory of matrix A and B : 0.000119 s ?
* CUBLAS : 

### GFLOPS
* Customize C : 
* Customize CUDA kernel 
	* Ordinary Kernel :
	* Shared memory of matrix A :
	* Shared memory of matrix A and B :
* CUBLAS : 

## Dependency
* Ubuntu 16.04
* CPU : Intel(R) Core(TM) i5-2400 CPU @ 3.10GHz
* GPU : GeForce GT 1030 
* CUDA version 9.0