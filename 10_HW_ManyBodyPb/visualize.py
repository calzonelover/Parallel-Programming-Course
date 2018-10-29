import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

N_PAR = 10
NT = 1000
PERIOD_SNAP = 1
img_file = []

for tp_i in range(int(NT/PERIOD_SNAP)):
	f_dat = "result_serial/Snapshot_t_%d.dat"%(tp_i*PERIOD_SNAP)
	x = np.genfromtxt(f_dat)[:,0]
	y = np.genfromtxt(f_dat)[:,1]
	plt.scatter(x,y)
	plt.title("Particles at time=%d"%(tp_i*PERIOD_SNAP))
	plt.xlabel("x")
	plt.ylabel("y")
	plt.savefig("image/Snapshot_t_%d.png"%(tp_i*PERIOD_SNAP))
	plt.clf()
	img_file.append("image/Snapshot_t_%d.png"%(tp_i*PERIOD_SNAP))

