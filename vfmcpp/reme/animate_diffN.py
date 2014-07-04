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
	while line:
		data.append(line)
		line = file.readline()
	file.close()
	r = np.zeros((len(data),3))
	for j in range(len(data)):
		r[j] = data[j].split()
	x = r[:,0]
	y = r[:,1]
	z = -r[:,2]

	for ring in rings:
		ring.set_data(x, y)
		ring.set_3d_properties(z)

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
	xlim=(-2.0e-6,2.0e-6), ylim=(-2.0e-6,2.0e-6), zlim=(0,1e-5), 
	aspect='equal',
	xticks=[], yticks=[],
	zticks=[0,1e-5],
	zticklabels=[0,10],
	projection='3d')

rings = []
rings += ax.plot([],[],[], 'b.', markersize=4)
time_text = ax.text(0.0, 0.0, 0.0, '', transform=ax.transAxes)
# ax.set_xlim3d((-2.0e-6,2.0e-6))
# ax.set_ylim3d((-2.0e-6,2.0e-6))
# ax.set_zlim3d((0,1e-5))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z (um)')
ax.view_init(10,0)
print len(files)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 10, blit=False)
plt.show()

		
	


