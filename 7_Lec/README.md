# Lecture 7
* Before using should check platform available
* OpenCL accepts float only single precision : 16bit float

## Tips
* uint get_work_dim() = grid in CUDA

## Hint Midterm-Exam
* Bandwidth : bus width, dat transfer per clock cycle, bus clock rate
* Latency or Response TIme
* CPU performance ~ 1/exec_time
* Application Parallelism
	* Data-level : many task item process same operation
	* Task-level : many task of work can be process indep , equiv to data-level iff each task have same operation
* Parallel Architectures
	* Instruction-level parallelism: data-level using pipelining
	* Core-level: multicore
	* Thread-level: what we do in GPU
	* Request-level: need warehouse cluster
* Flynn's Classification
* Tips: Program is a set of instruction
* Hardware/Software Categorized
	* Parallel computing: multiple execute at same time
	* Concurrent : compute at different or overlapse time
* Classification based on memory access
* Classification based on grain size
	* Grain size = num of instruction exec by processor before communication occur
	* Divided into 4 typed from number of operation
* Definition of speed up factor
* Scalable Programming model: Problem could be partitioned into indep solve and feed to blocks of threads
* Tips: SM = streaming multiprocessor
* Keyword: Kernel, Grid, Block
* Exam will write some CUDA code => need to remember simple syntax as threadId, blockId
* Memory Hierarchy: local, global => need to know different between convention of CUDA and OponCL
* Synchronization: __syncthreads() = barrier()
* Warp
	* warp exec one common instruction at a time
	* High performance when??
* Memory Optimization











