import os.path
import numpy as np
import matplotlib as matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import sys

base_filename = '../../data/offset_15R/'

def getfiles(base_filename):
	files = list()
	times = list()
	k = 0
	j = 0
	jmax = 0

	end = False
	end2 = False
	f = 0
	while(end==False):
		end2 = False
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
			print ('File read successful for ' + base_filename)

	return files, times, jmax

def data_read(files, times, jmax):
	i = 0
	f = 0
	trajx = []
	trajy = []
	trajz = []
	line_rx = []
	line_ry = []
	while(f<len(files)):
		end = False
		r = []
		m = 0
		fil = 0
		while(end == False):
			#print(files[f]+'_'+str(fil)+'.dat')
			if os.path.isfile(files[f]+'_'+str(fil)+'.dat') == True:
				data = []
				file = open(files[f]+'_'+str(fil)+'.dat', 'r')
				data = file.readlines()
				data = np.delete(data,0)
				file.close()
				if(data[0] == data[-1]):
					r.append(np.zeros((3, len(data))))
					for j in range(len(data)):
						for q in range(3):
							r[m][q][j] = data[j].split()[q]
					m += 1
					fil += 1
				else:
					fil += 1
					if(f>0 and f%20==0):
						for datum in data:
							line_rx.append(datum.split()[0])
							line_ry.append(datum.split()[1])            
			else:
				trajx.append(np.zeros((m)))
				trajy.append(np.zeros((m)))
				trajz.append(np.zeros((m)))
				for q in range(m):
					trajx[i][q] = np.mean(r[q][0][:])
					trajy[i][q] = np.mean(r[q][1][:])
					trajz[i][q] = np.mean(r[q][2][:])
				i += 1
				f += 1
				end = True
	return trajx, trajy, line_rx, line_ry

def add_to_plot(fig, ax, trajx, trajy, line_rx, line_ry,color):
	#colors = plt.cm.gnuplot(np.linspace(0,0.8,len(trajx)))
	for a in range(len(trajx)):
		ax.plot(trajx[a][:], trajy[a][:], linewidth=0, markersize=4, marker='o', alpha=0.5, c=color)
	#colors2 = plt.cm.gnuplot_r(np.linspace(0.2,1,len(line_rx)))
	#line_rx = np.flipud(line_rx)
	#line_ry = np.flipud(line_ry)
	#ax.scatter(line_rx, line_ry, marker='o', s=2, alpha=0.2, lw=0.01, c=color)
	return fig, ax


fig = plt.figure(figsize=(6,13),facecolor='k')
ax = fig.add_subplot(111,aspect='equal',axisbg='k')




folder_list = list()

for root, dirs, files in os.walk(base_filename):
	for name in dirs:
		folder_list.append(name)

folder_array = np.zeros(len(folder_list), dtype=np.float64)
i = 0
for folder in folder_list:
    folder_array[i] = folder_list[i].split('_')[1]
    i += 1

inds = folder_array.argsort()

temp_list = list()
for a in range(len(folder_list)):
	temp_list.append(folder_list[inds[a]])

folder_list = temp_list

colors = plt.cm.gnuplot(np.linspace(0,0.8,len(folder_list)))
a = 0
while(a<len(folder_list)):
	filename = base_filename + folder_list[a] + "/data_"
	files, times, jmax = getfiles(filename)
	trajx, trajy, line_rx, line_ry = data_read(files, times, jmax)
	fig, ax  = add_to_plot(fig, ax, trajx, trajy, line_rx, line_ry, colors[a])
	a += 1

ax.set_xlim(-5e-6,5e-6)
ax.set_ylim(-6e-6,10e-6)
ax.set_xticklabels([])
ax.set_yticklabels([])
ax.axis('off')
print('Saving image...')
plt.savefig('multi_trajectory.png')
#plt.show()
print('image saved.')



