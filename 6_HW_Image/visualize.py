import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

x = np.genfromtxt("histogram_gpu.dat")

plt.plot(x)
plt.xlabel("Pixel number")
plt.ylabel("Count")
plt.show()