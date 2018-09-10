# HW : Matrix Multiplication
## Matrix multiplication 
* For a given matrices A and B with dimension (WA, HA) (WB, HB) sequentially
* The product from multiplication C would has dimension (WB, HA)
* This relation can be written as <img src="https://latex.codecogs.com/gif.latex?C_{i,j}&space;=&space;\sum_{d}^{\text{WA}\equiv&space;\text{HB}}&space;A_{d,j}\cdot&space;B_{i,d}" title="C_{i,j} = \sum_{d}^{\text{WA}\equiv \text{HB}} A_{d,j}\cdot B_{i,d}" />

## Methods
* Pure cutomize function (CPU)
* With customize kernel CUDA (GPU)
* With libraly CUBLAS (GPU)

## Timing Results
* Customize C : 
* Customize CUDA kernel : 
	* Ordinary Kernel :
	* Shared memory of matrix A :
	* Shared memory of matrix A and B :
* CUBLAS : 

## GFLOPS
* Customize C : 
* Customize CUDA kernel : 
	* Ordinary Kernel :
	* Shared memory of matrix A :
	* Shared memory of matrix A and B :
* CUBLAS : 