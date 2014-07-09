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
	base_filename = '../bin/data/dat_PLEASE_PLEASE_ADJUST_ME/data_'
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

	while line:
		data.append(line)
		line = file.readline()
	file.close()
	r = np.zeros((len(data),3))
	for j in range(len(data)):
		r[j] = data[j].split()
	x = np.zeros(len(r)+1)
	y = np.zeros(len(r)+1)
	z = np.zeros(len(r)+1)
	for n in range(len(r)):	
		x[n] =  r[n,0]
		y[n] =  r[n,1]
		z[n] =  r[n,2]

	x[-1] = x[0]
	y[-1] = y[0]
	z[-1] = z[0]

	

	rings[0].set_data(x, y)
	rings[0].set_3d_properties(z)


	time_text.set_text('time = %.1f' % (1e9*i*N_f*dt)+ ' ns / %.1f' % (len(files)*1e9*N_f*dt) +' ns')
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 10000
dt = 9.1e-11
files = getfiles(N_f)
fig = plt.figure(facecolor='w',figsize=(10,10))
ax = fig.add_subplot(111, 
	aspect='equal',
	xticks=[0], yticks=[0], zticks=[0],
	projection='3d')

rings = []
rings += ax.plot([],[],[], 'g.', markersize=2, linewidth=2, alpha=0.5)
#rings += ax.plot([],[],[], 'b-', markersize=2, linewidth=2, alpha=0.5)
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes)
ax.set_xlim3d((-8.0e-6,8.0e-6))
ax.set_ylim3d((-8.0e-6,8.0e-6))
ax.set_zlim3d((0,10e-5))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z (um)')
ax.view_init(10,0)
print len(files)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 1, blit=False)
plt.show()