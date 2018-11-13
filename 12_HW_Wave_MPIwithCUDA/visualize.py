import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.cm as cm

temp = np.loadtxt('wave2d_cudampi_map.dat').T
# temp = np.loadtxt('wave2d_map.dat').T
nrows, ncols = 202, 202
x = [i for i in range(nrows)]
u = [i for i in range(ncols)]

grid = temp.reshape((nrows, ncols))

plt.imshow(grid, 
           interpolation='nearest', cmap=cm.gist_rainbow)
           
plt.savefig("wave2d_cuda_t1000.png")
# plt.show()
