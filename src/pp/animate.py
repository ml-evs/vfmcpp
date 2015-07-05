import os.path
import numpy as np
import matplotlib
#matplotlib.use('Agg')
import matplotlib.pyplot as plt
import sys
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
#import pandas as pd
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
		data_folder = input('Enter data folder path:')
		base_filename = data_folder + '/data_'

	end = False
	end2 = False
	f = 0
	while(end==False):
		end2 = False
		filename_k = base_filename + str(k)
		if os.path.isfile(filename_k+"_0.dat") == True:
			file = open(filename_k+'_0.dat', 'r')
			line = file.readline()
			if f==0:
				for i in range(35):
					times.append(float(line))
					files.append(filename_k)
				f=1
			else:
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
			print ('File read successful. ')
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
			r.append(np.zeros((len(data),3)))
			for j in range(len(data)):
				r[m][j] = data[j].split()
			m+=1
		else:
			for b in range(m,len(rings)):
				rings[b].set_data([],[])
				rings[b].set_3d_properties([])
			end = True

	q_biggest = 0
	for q in range(len(r)):
		test = max(r[q][:,0])-min(r[q][:,0])
		if test > max(r[q_biggest][:,0])-min(r[q_biggest][:,0]):
	 		q_biggest = q
	z_av = np.mean(r[q_biggest][:,2])
	for q in range (len(r)):
		rings[q].set_data(r[q][:,0], r[q][:,1])
		rings[q].set_3d_properties(r[q][:,2])#-z_av)


	time_text.set_text('time = %.1f' % (times[i]*1e9)+ ' ns / %.1f' % (times[-1]*1e9) +' ns')
	fig.canvas.draw()
	plt.draw()
	return rings, time_text

base_filename, files, times, jmax = getfiles()

if len(sys.argv) > 2:
	Analysis = str(sys.argv[2])
else:
	Analysis = input('[p]lot to screen, [g]if or [m]ovie?')


fig = plt.figure(facecolor='None', edgecolor='None',figsize=(4,4))
ax = fig.add_subplot(111,
 	aspect='equal',
 	axisbg='None',
 	xticks=[], yticks=[], zticks=[],
 	xticklabels=[], yticklabels=[], zticklabels=[],
 	projection='3d')

fig.subplots_adjust(hspace=0)

rings = []
print (jmax)
colors = plt.cm.PuOr(np.linspace(0,1,2))
style = '-'
#if Analysis == 'p':
#	style = '-'

for k in range (jmax+2):
	rings += [l for c in colors for l in ax.plot([], [], [], style, c=np.random.rand(3,1), alpha = 0.9, linewidth=2, markersize=5, markerfacecolor=c, markeredgecolor=c)]
	#rings += [l for c in colors for l in ax.plot([], [], [], style, c='k', alpha = 1, linewidth=3, markersize=5, markerfacecolor='k', markeredgecolor='k')]
time_text = ax.text(0, 0, 0,'', transform=ax.transAxes, color='k')
ax.set_xlim3d((-3e-6,3e-6))
ax.set_ylim3d((-3e-6,3e-6))
ax.set_zlim3d((-3e-6,3e-6))
#ax.set_zlim3d((24e-6,27e-6))
ani = animation.FuncAnimation(fig, animate, repeat=False, init_func = init, frames = len(files), interval = 1, blit=False)

if Analysis == 'p':
	plt.show()
elif Analysis == ('g' or 'm'):
	if len(sys.argv) > 3:
		filename = '../../img/ ' + sys.argv[3]
	else:
		filename = input('Enter the desired filename for the gif (will be saved in data folder):')
		filename = '../../post/' + filename
	if Analysis == 'g':
		print (filename)
		ani.save(filename+'.gif', writer='imagemagick', fps=20, dpi=100)

	elif Analysis == 'm':
		ani.save(filename+'.mp4', fps=20, dpi=500)
