import os.path
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

def init():
	time_text.set_text('')
	for ring in rings:
		ring.set_data([], [])
		ring.set_3d_properties([])
	return rings, time_text

def getfiles(N_f):
	files = list()
	times = list()
	k = 10
	j = 0
	jmax = 0
	base_filename = '../bin/data/015_09_self/data_'
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
	r = []
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
			r.append(np.zeros((len(data)+1,3)))
			for j in range(len(data)):
				r[m][j] = data[j].split()
			r[m][-1] = r[m][0]
			m+=1
			#ax.relim()
		else:
			rings[m].set_data([],[])
			rings[m].set_3d_properties([])
			end = True

	q_biggest = 0
	for q in range(len(r)):
		test = max(r[q][:,0])-min(r[q][:,0])
		if test > max(r[q_biggest][:,0])-min(r[q_biggest][:,0]):
			q_biggest = q
	z_av = np.mean(r[q_biggest][:,2])
	for q in range (len(r)):
		rings[q].set_data(r[q][:,0], r[q][:,2]-z_av)
		rings[q].set_3d_properties(r[q][:,1])

	time_text.set_text('time = %.1f' % (times[i]*1e9)+ ' ns / %.1f' % (times[-1]*1e9) +' ns')
	ax.view_init(10,-130+0.1*i)
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

N_f = 10000
dt = 1.51828e-11

files, times, jmax = getfiles(N_f)
fig = plt.figure(figsize=plt.figaspect(2.), facecolor='w', edgecolor='w')
ax = fig.add_subplot(111, 
 	aspect='equal',
 	axisbg='w',
 	xticks=[], yticks=[], zticks=[],
 	xticklabels=[], yticklabels=[], zticklabels=[],
	projection='3d')
rings = []
colors = plt.cm.Greens(np.linspace(0.8,1, jmax))
for k in range (jmax+2):
	rings += [l for c in colors for l in ax.plot([], [], [],'-', c=c, alpha = 0.9, linewidth=4)]
time_text = ax.text(0.0, 0.0, 0, '', transform=ax.transAxes, color='k')
ax.set_xlim3d((-1.5e-6,1.5e-6))
#plt.axis('off')
ax.set_ylim3d((-1.5e-6,1.5e-6))
ax.set_zlim3d((-1.5e-6,1.5e-6))
ax.view_init(10,-130)

x = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.2e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="r")
y = Arrow3D([-1.2e-6,-0.8e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="g")
z = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-0.8e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="#2E5C99")
ax.add_artist(x)
ax.add_artist(y)
ax.add_artist(z)
ani = animation.FuncAnimation(fig, animate, init_func = init, frames = len(files), interval = 1, blit=False)

#ani.save('../img/015_09.gif', writer='imagemagick', fps=20);
#ani.save('reconnection.mp4', fps=30, dpi=500)

plt.show()