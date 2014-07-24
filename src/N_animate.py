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
	times = list()
	k = 0
	j = 0
	jmax = 0
	base_filename = '../bin/data/init_test_new/data_'
	end = False
	end2 = False
	while(end==False):
		filename_k = base_filename + str(k)
		if os.path.isfile(filename_k+"_0.dat") == True:
			file = open(filename_k+'_0.dat', 'r')
			line = file.readline()
			times.append(float(line))
			files.append(filename_k)
			k+=1
			while(end2 == False):
				if os.path.isfile(filename_k+"_"+str(j)+".dat") == True:
					j += 1
				else:
					if j>jmax:
						jmax = j
						end2 = True
		else:
			end = True
	return files, times, jmax

def animate(i):

	end = False
	m = 0
	z_av = np.zeros((jmax+1))
	while(end == False):
		if os.path.isfile(files[i]+'_'+str(m)+'.dat') == True:
			data = []
			file = open(files[i]+'_'+str(m)+'.dat', 'r')
			line = file.readline()
			line = file.readline() #skip first line
			while line: 
				data.append(line)
				line = file.readline()
			file.close()
			r = np.zeros((len(data)+1,3))
			for j in range(len(data)):
				r[j] = data[j].split()
			r[-1] = r[0]
			z_av[m] = r[0,2]
			rings[m].set_data(r[:,0], r[:,1])
			rings[m].set_3d_properties(r[:,2]-z_av[0])
			m+=1
			ax.relim()
		else:
			rings[m].set_data([],[])
			rings[m].set_3d_properties([])
			end = True

	time_text.set_text('time = %.1f' % (times[i]*1e9)+ ' ns / %.1f' % (times[-1]*1e9) +' ns')
	ax.autoscale_view()
	#ax.view_init(10,90+0.5*i)
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 10000
dt = 1.51828e-11

files, times, jmax = getfiles(N_f)
fig = plt.figure(figsize=(10,10))
ax = Axes3D(fig)
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])
ax.set_aspect('equal')
# ax = fig.add_subplot(111, 
# 	aspect='equal',
# 	xticks=[0], yticks=[0], zticks=[0],
# 	projection='3d')
rings = []
jmax += 2
colors = plt.cm.winter(np.linspace(0,1, jmax))
for k in range (jmax+2):
	rings += [l for c in colors for l in ax.plot([], [], [], '-', c=c, alpha = 1, linewidth=4)]
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes)
ax.set_xlim3d((-2.0e-6,2.0e-6))
ax.set_zlim3d((-5e-6,5e-6))
ax.set_ylim3d((-2.0e-6,2.0e-6))
ax.set_xlabel('x')
ax.set_ylabel('z (um)')
ax.set_zlabel('y')
ax.view_init(10,90)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 100, blit=False)
#ani.save('reconnection.mp4', fps=30, dpi=500)

plt.show()