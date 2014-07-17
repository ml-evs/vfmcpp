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
	j = 0
	k = 0
	jmax = 0
	base_filename = '../bin/data/full_whammy015/data_'
	end = False
	end2 = False
	while(end==False):
		filename_i = base_filename + str(i)
		filename_k = base_filename + str(k)
		if os.path.isfile(filename_k+"_0.dat") == True:
			files.append(filename_k)
			k+=1
			while(end2 == False):
				if os.path.isfile(filename_k+"_"+str(j)+".dat") == True:
					j += 1
				else:
					if j>jmax:
						jmax = j
						end2 = True
		elif os.path.isfile(filename_i+"_0.dat") == True:
			files.append(filename_i)
			i += N_f
			k = i
			while(end2 == False):
				if os.path.isfile(filename_i+"_"+str(j)+".dat") == True:
					j += 1
				else:
					if j>jmax:
						jmax = j
						end2 = True
		else:
			end = True
	return files, jmax

def animate(i):

	end = False
	m = 0
	while(end == False):
		if os.path.isfile(files[i]+'_'+str(m)+'.dat') == True:
			data = []
			file = open(files[i]+'_'+str(m)+'.dat', 'r')
			line = file.readline()
			while line: 
				data.append(line)
				line = file.readline()
			file.close()
			r = np.zeros((len(data)+1,3))
			for j in range(len(data)):
				r[j] = data[j].split()
			r[-1] = r[0]
			rings[m].set_data(r[:,0], r[:,1])
			rings[m].set_3d_properties(r[:,2])
			m+=1
			ax.relim()
		else:
			end = True

	time_text.set_text('time = %.1f' % (1e9*i*N_f*dt)+ ' ns / %.1f' % (len(files)*1e9*N_f*dt) +' ns')
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 10000
dt = 1.51828e-10
files, jmax = getfiles(N_f)
fig = plt.figure(facecolor='w',figsize=(10,10))
ax = fig.add_subplot(111, 
	aspect='equal',
	xticks=[0], yticks=[0], zticks=[0],
	projection='3d')

rings = []
colors = plt.cm.jet(np.linspace(0, 1, jmax))
print jmax
for k in range (jmax):
	rings += [l for c in colors for l in ax.plot([], [], [], '-', c=c, alpha = 1, linewidth=4)]
print len(rings)
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes)
ax.set_xlim3d((-4.0e-6,4.0e-6))
ax.set_ylim3d((-4.0e-6,4.0e-6))
ax.set_zlim3d((0,10e-5))
ax.autoscale_view(tight=True, scalex=False, scaley=False, scalez=True)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z (um)')
ax.view_init(10,0)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 1, blit=False)
plt.show()