import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

temp = np.loadtxt('wave2d_map.dat').T #Transposed for easier unpacking
nrows, ncols = 256, 256
x = [i for i in range(nrows)]
u = [i for i in range(ncols)]

grid = temp.reshape((nrows, ncols))

plt.imshow(grid, 
           interpolation='nearest', cmap=cm.gist_rainbow)
plt.show()