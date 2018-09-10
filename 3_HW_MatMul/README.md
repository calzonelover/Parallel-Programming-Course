# HW : Matrix Multiplication
## Matrix multiplication 
* For a given matrices A and B with dimension (WA, HA) (WB, HB) sequentially
* The product from multiplication C would has dimension (WB, HA)
* This relation can be written as 
* ![\Large C_{i,j} = \sum_{d}^{\text{WA}\equiv \text{HB}} A_{d,j}\cdot B_{i,d}](https://latex.codecogs.com/svg.latex?x%3D%5Cfrac%7B-b%5Cpm%5Csqrt%7Bb%5E2-4ac%7D%7D%7B2a%7D)

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