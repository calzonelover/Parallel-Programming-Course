import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

x = np.genfromtxt("5_data.dat")[1:,1]
t_s = np.genfromtxt("5_data.dat")[1:,2]
t_p = np.genfromtxt("5_data.dat")[1:,3]
s = np.divide(t_s,t_p)

x = np.power(2, x)
print(x, s)

plt.plot(x, s)
plt.title("Same problem size per process")
plt.xlabel("Number Processes")
plt.ylabel("Speed up factor")
plt.show()