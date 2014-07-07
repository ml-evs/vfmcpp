import os.path
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D

def init():
	time_text.set_text('')
	for ring in rings:
		ring.set_data([], [])
		ring.set_3d_properties([])
	return rings, time_text

def getfiles(N_f):
	files = list()
	i = 0
	base_filename = '../bin/data/dat3_rings_diff/data_'
	end = False
	while(end==False):
		filename = base_filename + str(i) + '.dat'
		if os.path.isfile(filename) == True:
			files.append(filename)
			i += N_f
		else:
			end = True
	return files

def animate(i):

	file = open(files[i], 'r')
	line = file.readline()
	data = []
	x1 = np.zeros((101))
	x2 = np.zeros((101))
	x3 = np.zeros((101))
	y1 = np.zeros((101))
	y2 = np.zeros((101))
	y3 = np.zeros((101))
	z1 = np.zeros((101))
	z2 = np.zeros((101))
	z3 = np.zeros((101))
	while line:
		data.append(line)
		line = file.readline()
	file.close()
	r = np.zeros((len(data),3))
	for j in range(len(data)):
		r[j] = data[j].split()
	for n in range(100):	
		x1[n] =  r[n,0]
		y1[n] =  r[n,1]
		z1[n] = -r[n,2]
		x2[n] =  r[n+100,0]
		y2[n] =  r[n+100,1]
		z2[n] = -r[n+100,2]
		x3[n] =  r[n+200,0]
		y3[n] =  r[n+200,1]
		z3[n] = -r[n+200,2]
	x1[-1] = x1[0]
	x2[-1] = x2[0]
	y1[-1] = y1[0]
	y2[-1] = y2[0]
	z2[-1] = z2[0]
	z1[-1] = z1[0]
	y3[-1] = y3[0]
	z3[-1] = z3[0]
	x3[-1] = x3[0]

	rings[0].set_data(x1, y1)
	rings[0].set_3d_properties(z1)
	rings[1].set_data(x2, y2)
	rings[1].set_3d_properties(z2)
	rings[2].set_data(x3, y3)
	rings[2].set_3d_properties(z3)

	#ax.view_init(10, 0.3 * i)
	time_text.set_text('time = %.1f' % (1e9*i*N_f*dt)+ ' ns / %.1f' % (len(files)*1e9*N_f*dt) +' ns')
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 5000
dt = 9.1e-11
files = getfiles(N_f)
fig = plt.figure(facecolor='w',figsize=(10,10))
ax = fig.add_subplot(111, 
	xlim=(-4.0e-6,4.0e-6), ylim=(-4.0e-6,4.0e-6), zlim=(0e-5,3e-5), 
	aspect='equal',
	xticks=[0], yticks=[0], zticks=[0],
	projection='3d')

rings = []
rings += ax.plot([],[],[], 'b-', markersize=2, linewidth=3, alpha=0.8)
rings += ax.plot([],[],[], 'g-', markersize=2, linewidth=3, alpha=0.8)
rings += ax.plot([],[],[], 'c-', markersize=2, linewidth=3, alpha=0.8)
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes)
ax.set_xlim3d((-2.0e-6,2.0e-6))
ax.set_ylim3d((-2.0e-6,2.0e-6))
ax.set_zlim3d((0,3e-5))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z (um)')
ax.view_init(10,0)
print len(files)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 10, blit=False)
plt.show()