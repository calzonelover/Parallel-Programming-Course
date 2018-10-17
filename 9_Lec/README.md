# Lecture 9

## Cache
* A cache is a component that stores data
	* Cache hit : served by simply reading the cache
	* Cache miss : data has to be recomputed or fetched/read from its original storage 
* Cache miss slower than cache hit
* Tips: also depends on program that array store row or column major
* From test in c program 8192x8192 array we get 
	* i then j elapse_time : 220 ms
	* j then i elapse_time : 788 ms
	* The reson is simply because C language store matrix as row first then could really see effect of "Cache hit" and "Cache miss"
* Note: array2d.c need to put one number argument when W = H eg. "./array2d 8000"


## Master-Slave
Divide problem into strip which rank-0 sending job and run on slave(other rank)
* If rank 0 is old machine even we have cool cluster but it has bottleneck problem => load balancing to solving

## Elementary Scalability Calculation
Ref[0] p.39, What best efficientcy for numprocesses to divide problem by consider T_comm and T_calc

## Ref
* [0] https://www.mcs.anl.gov/research/projects/mpi/usingmpi/examples-usingmpi/index.html