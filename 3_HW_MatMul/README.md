# HW : Matrix Multiplication
## Matrix multiplication 
* For a given matrices A and B with dimension (WA, HA) (WB, HB) sequentially
* The product from multiplication C would has dimension (WB, HA)
* This relation can be written as  <br/>
	<img style="text-align: center;" src="https://latex.codecogs.com/gif.latex?C_{i,j}&space;=&space;\sum_{d}^{\text{WA}\equiv&space;\text{HB}}&space;A_{d,j}\cdot&space;B_{i,d}" title="C_{i,j} = \sum_{d}^{\text{WA}\equiv \text{HB}} A_{d,j}\cdot B_{i,d}" />
* Note that all matrix elements is all float number with dimension
	* (WA, HA) = (320, 320)
	* (WB, HB) = (640, 320)
	* Which cause (WC, HC) = (WB, HA) => there are 640*320*320=65,536,000 floating point operation
* 10 times iteration of calculation for more precise time elapsed due to we use a small matrix size
* Notice that dimension matching of matrices for multiplication have to have WA = HB and it also already take into account for checking process before calculation in this code

## Methods
* Pure cutomize function (CPU)
* Customize kernel CUDA (GPU) for unshared memory and shared memory method for both of input matrices

## FLOPs evaluation
* FLOPs calculation : cores * clockFrequency * 1/T  * (FLOPs/cycle)

## Results
### Timing
#### Matrix multiplication
* Customize C : 0.272511 s
* Customize CUDA kernel 
	* Ordinary Kernel : 0.003247 s
	* Shared memory of matrix A : 0.000205 s
	* Shared memory of matrix A and B : 0.000126 s
#### Wave2D
* Ordinary Kernel :  s
* Kernel with shared : s

### GFLOPS
#### Matrix multiplication
* Customize C : 
* Customize CUDA kernel 
	* Ordinary Kernel : 
	* Shared memory of matrix A :
	* Shared memory of matrix A and B :
#### Wave2D
* Ordinary Kernel :  
* Kernel with shared : 



## Dependency
* Ubuntu 16.04
* CPU : Intel(R) Core(TM) i5-2400
	* 3.1 Ghz
	* 4 Cores
* GPU : GeForce GT 1030 
	* 384 CUDA cores
	* 1.38 GHz
* CUDA version 9.0
