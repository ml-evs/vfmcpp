import os.path
import numpy as np
import matplotlib.pyplot as plt
import sys
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
import matplotlib.gridspec as gridspec
import pandas as pd
import matplotlib

pd.options.display.mpl_style = 'default'

font = {'family' : 'monospace',
        'size'   : 8}

matplotlib.rc('font', **font)

class Line():
	def __init__(self):
		self.L = []
		self.dr = []

	def linelength(self, r):
		l = []
		for j in range (len(r)):
			l.append(0)
			for k in range(len(r[j])-1):
				temp = []
				for q in range(3):
					temp.append(pow(r[j][k][q]-r[j][k+1][q],2))
				tempsum = np.sqrt(sum(temp))
				l[j] += tempsum
		self.L.append(sum(l))

	def res(self,r):
		l = []
		no_points = 0
		for j in range (len(r)):
			l.append(0)
			for k in range(len(r[j])-1):
				no_points +=1
				temp = []
				for q in range(3):
					temp.append(pow(r[j][k][q]-r[j][k+1][q],2))
				l[j] += np.sqrt(sum(temp))
		self.dr.append((4.0/(3.0*no_points)) * sum(l))

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
	k = 0
	j = 0
	jmax = 0
	base_filename = '../bin/data/new_recon_test/data_'
	if len(sys.argv)!=1:
		base_filename = '../bin/data/' + str(sys.argv[1]) + '/data_'
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
	L = 0
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
		else:
			for b in range(m,len(rings)):
				rings[b].set_data([],[])
				rings[b].set_3d_properties([])
			Length.linelength(r)
			Length.res(r)
			# len_pts[0].set_data(times[0:int(i)+1],Length.L[0:int(i)+1])
			# res_pts[0].set_data(times[0:int(i)+1],Length.dr[0:int(i)+1])

			# len_line.set_yticks([0.95*Length.L[0], Length.L[0], 1.05*Length.L[0]])
			# len_line.set_yticklabels(['%.1f' % (0.97*Length.L[0]*1000000) + ' um', '%.1f' % (Length.L[0]*1000000), '%.1f' % (1.02*Length.L[0]*1000000) + ' um'])
			# len_line.set_ylim(0.95*min(Length.L), 1.25*max(Length.L))
			# len_line.yaxis.tick_left()
			#res_line.set_yticks([0.95*Length.dr[0], Length.dr[0], 1.05*Length.dr[0]])
			#res_line.set_yticklabels(['%.1f' % (0.95*Length.dr[0]*1000000000) + ' nm', '%.1f' % (Length.dr[0]*1000000000), '%.1f' % (1.05*Length.dr[0]*1000000000) + ' nm'])
			#res_line.set_ylim(0.9*min(Length.dr), 1.1*max(Length.dr))
			#for tl in res_line.get_yticklabels():
  				 # tl.set_color('r')

			end = True

	q_biggest = 0
	for q in range(len(r)):
		test = max(r[q][:,0])-min(r[q][:,0])
		if test > max(r[q_biggest][:,0])-min(r[q_biggest][:,0]):
			q_biggest = q
	z_av = np.mean(r[q_biggest][:,2])
	for q in range (len(r)):
		rings[q].set_data(r[q][:,0], r[q][:,1])
		rings[q].set_3d_properties(r[q][:,2]-z_av)

	time_text.set_text('time = %.1f' % (times[i]*1e6)+ ' us / %.1f' % (times[-1]*1e6) +' us')
	#ax.view_init(10,-130+0.1*i)
	fig.canvas.draw()
	plt.draw()
	return rings, time_text#, res_pts, len_pts

N_f = 10000
dt = 1.3856-10

Length = Line()
gs = gridspec.GridSpec(2, 1,height_ratios=[5,1])
files, times, jmax = getfiles(N_f)
fig = plt.figure(facecolor='w', edgecolor='w',figsize=plt.figaspect(2.))
ax = fig.add_subplot(gs[0], 
 	aspect='equal',
 	axisbg='w',
 	xticks=[], yticks=[], zticks=[],
 	xticklabels=[], yticklabels=[], zticklabels=[],
 	projection='3d')
## ax2 = fig.add_subplot(gs[1], 
# 	xticks=[0,times[-1]], xticklabels=[0, '%.1f' % (10000000*times[-1])],
# 	xlabel = 't (us)', 
# 	#ylabel = 'Filament length (um)',
# 	yticks =[], yticklabels = [],
#  	axisbg='w')

fig.subplots_adjust(hspace=0)
##res_line = ax2.twinx()
#len_line = ax2.twinx()
#res_line.set_ylabel('Resolution (nm)')
# len_line.set_ylabel('Filament length (nm)')
# len_line.yaxis.set_label_position("left")
rings = []
colors = plt.cm.Greens(np.linspace(0.8,1, jmax))
style = '-*'
if len(sys.argv) > 2:
	style = str(sys.argv[2])

# res_pts = []
# len_pts = []

#res_pts += res_line.plot([],[], linewidth=4, c='r')
#len_pts += len_line.plot([],[], linewidth=4, c='k')

for k in range (jmax+2):
	rings += [l for c in colors for l in ax.plot([], [], [], style, c=c, alpha = 0.9, linewidth=4, markersize=5, markerfacecolor=c, markeredgecolor='b')]
time_text = ax.text(1.0, 1.0, 1,'', transform=ax.transAxes, color='k')
ax.set_xlim3d((-1.5e-6,1.5e-6))
#ax2.set_xlim((0,max(times)))
## ax2.set_ylim((8.4e-7, 8.5e-7))
ax.set_ylim3d((-1.5e-6,1.5e-6))
ax.set_zlim3d((-1.5e-6,1.5e-6))
ax.view_init(10,-130)

x = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.2e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="r")
y = Arrow3D([-1.2e-6,-0.8e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="g")
z = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-0.8e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="#2E5C99")
ax.add_artist(x)
ax.add_artist(y)
ax.add_artist(z)
ani = animation.FuncAnimation(fig, animate, repeat=False, init_func = init, frames = len(files), interval = 100, blit=False)

#ani.save('', writer='imagemagick', fps=20);
#ani.save('reconnection.mp4', fps=30, dpi=500)

plt.show()