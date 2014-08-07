import os.path
import numpy as np
import matplotlib.pyplot as plt
import sys
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
import pandas as pd
import matplotlib
from pp import Line, Arrow3D


def getfiles():
	files = list()
	times = list()
	k = 0
	j = 0
	jmax = 0
	if len(sys.argv)!=1:
		base_filename = '../../data/' + str(sys.argv[1]) + '/data_'
	else:
		data_folder = raw_input('Enter data folder path:')
		base_filename = data_folder + '/data_'

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
			print 'File read successful. '
	return base_filename, files, times, jmax


def init():
	time_text.set_text('')
	for ring in rings:
		ring.set_data([], [])
		ring.set_3d_properties([])
	return rings, time_text


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
	#ax.view_init(20,-130+0.2*i)
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

dt = 1.3856-10
base_filename, files, times, jmax = getfiles()

if len(sys.argv) > 2:
	Analysis = str(sys.argv[2])
else:
	Analysis = raw_input('[p]lot to screen, [g]if or [m]ovie?')


Length = Line()
fig = plt.figure(facecolor='w', edgecolor='w',figsize=plt.figaspect(1.))
ax = fig.add_subplot(111, 
 	aspect='equal',
 	axisbg='w',
 	xticks=[], yticks=[], zticks=[],
 	xticklabels=[], yticklabels=[], zticklabels=[],
 	projection='3d')

fig.subplots_adjust(hspace=0)

rings = []
colors = plt.cm.Blues(np.linspace(1,0.8, jmax))
style = '-'
if Analysis == 'p':
	style = '-*'

for k in range (jmax+2):
	rings += [l for c in colors for l in ax.plot([], [], [], style, c=c, alpha = 0.9, linewidth=4, markersize=5, markerfacecolor=c, markeredgecolor=c)]
time_text = ax.text(0, 0, 0,'', transform=ax.transAxes, color='k')
ax.set_xlim3d((-1.5e-6,1.5e-6))
ax.set_ylim3d((-1.5e-6,1.5e-6))
ax.set_zlim3d((-1.5e-6,1.5e-6))
ax.view_init(20,-130)

x = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.2e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="r")
y = Arrow3D([-1.2e-6,-0.8e-6],[-1.2e-6,-1.2e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="g")
z = Arrow3D([-1.2e-6,-1.2e-6],[-1.2e-6,-0.8e-6],[-1.6e-6,-1.6e-6], mutation_scale=20, lw=2, arrowstyle="-|>", color="#2E5C99")
ax.add_artist(x)
ax.add_artist(y)
ax.add_artist(z)

ani = animation.FuncAnimation(fig, animate, repeat=True, init_func = init, frames = len(files), interval = 1, blit=False)

if Analysis == 'p':
	plt.show()
elif Analysis == ('g' or 'm'):
	if len(sys.argv) > 3:
		filename = '../../img/ ' + sys.argv[3]
	else:
		filename = raw_input('Enter the desired filename for the gif (will be saved in data folder):')
		filename = '../../img/' + filename
	if Analysis == 'g':
		ani.save(filename+'.gif', writer='imagemagick', fps=20)
	elif Analysis == 'm':
		ani.save(filename+'.mp4', fps=20, dpi=500)
