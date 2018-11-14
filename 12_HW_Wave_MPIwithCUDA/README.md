# HW : CUDA with MPI for 2D-Wave Halo Technique
# Mr. Patomporn Payoungkhamdee SCPY 6138171

## Method
* Fixed 2 static processes with their own initialize wave2d map
* compute and update halo over time between these two processes by synchoronous halo line at their boundary

## Results
### 1) MPI
Found that ordinary MPI produce a satisfied result we could see in t_0 and t_1000 in the following images
<p align="center">
<img src="wave2d_t0.png" width="500px" >
</p>
<p align="center">
<img src="wave2d_t1000.png" width="500px" >
</p>

### 2) MPI with CUDA
Performing the same logic and write function "stepWave" and "updateWave" in cuda file rather than write function in c file and partially compile by setting it available external call from MPI program the results at time t_100 and t_1000 has shown in below
<p align="center">
<img src="wave2d_cuda_t100.png" width="500px" >
</p>
<p align="center">
<img src="wave2d_cuda_t1000.png" width="500px" >
</p>
