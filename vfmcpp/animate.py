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
	base_filename = 'dat/data_'
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
	y1 = np.zeros((101))
	y2 = np.zeros((101))
	z1 = np.zeros((101))
	z2 = np.zeros((101))
	while line:
		data.append(line)
		line = file.readline()
	file.close()
	r = np.zeros((len(data),3))
	for j in range(len(data)):
		r[j] = data[j].split()
	for i in range(100):	
		x1[i] =  r[2*i,0]
		y1[i] =  r[2*i,1]
		z1[i] = -r[2*i,2]
		x2[i] =  r[2*i+1,0]
		y2[i] =  r[2*i+1,1]
		z2[i] = -r[2*i+1,2]
	x1[-1] = x1[0]
	x2[-1] = x2[0]
	y1[-1] = y1[0]
	y2[-1] = y2[0]
	z2[-1] = z2[0]
	z1[-1] = z1[0]

	rings[0].set_data(x1, y1)
	rings[0].set_3d_properties(z1)
	rings[1].set_data(x2, y2)
	rings[1].set_3d_properties(z2)
	
	#ax.view_init(10, 0.3 * i)
	time_text.set_text('time = %.1f' % (1e9*i*N_f*dt) + ' ns / %.1f' % (len(files)*1e9*N_f*dt) +' ns')
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 2000
dt = 9.1e-11
files = getfiles(N_f)
fig = plt.figure(facecolor='w',figsize=(10,10))
ax = fig.add_subplot(111, 
	xlim=(-2.0e-6,2.0e-6), ylim=(-2.0e-6,2.0e-6), zlim=(0,5e-5), 
	aspect='equal',
	xticks=[], yticks=[], zticks=[],
	#zticks=[0,1e-7,2e-7,3e-7,4e-7,5e-7],
	#zticklabels=[0,0.1,0.2,0.3,0.4,0.5],
	projection='3d')

rings = []
rings += ax.plot([],[],[], 'g-', markersize=2, linewidth=2)
rings += ax.plot([],[],[], 'b-', markersize=2, linewidth=2)
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes)
ax.set_xlim3d((-2.0e-6,2.0e-6))
ax.set_ylim3d((-2.0e-6,2.0e-6))
ax.set_zlim3d((0,5e-5))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z (um)')
ax.view_init(10,0)
print len(files)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 10, blit=False)
plt.show()

		
	


