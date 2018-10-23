import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

x = np.genfromtxt("2_data.dat")[:,0]
y_s = np.genfromtxt("2_data.dat")[:,1]
y_d = np.genfromtxt("2_data.dat")[:,2]

print(x, y_s, y_d)

plt.plot(x, y_d)
plt.xlabel("Number Processes")
plt.ylabel("Time")
plt.show()