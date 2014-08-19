import sys
import os.path
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

pd.options.display.mpl_style = 'default'

font = {'family' : 'monospace',
        'size'   : 8}

matplotlib.rc('font', **font)

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

	print base_filename	
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
			print str(len(files)) + ' files read successfully. ' 
	return base_filename, files, times, jmax

def calcimpulse(jmax, impulse_files, i):

	kappa = 9.98e-8

	end = False
	m=0
	r = []
	sp = []
	l= []
	eps = []
	rxeps = []
	r_eff = []
	radius = np.zeros((jmax+1))
	p = np.zeros((jmax,3))

	while(end == False):
		if os.path.isfile(impulse_files[i]+'_'+str(m)+'.dat') == True:
			data = []
			file = open(impulse_files[i]+'_'+str(m)+'.dat', 'r')
			line = file.readline()
			line = file.readline() #skip first line
			while line: 
				data.append(line)
				line = file.readline()
			file.close()
			r.append(np.zeros((len(data),3)))
			l.append(np.zeros((len(data),3)))
			eps.append(np.zeros((len(data),3)))
			rxeps.append(np.zeros((len(data),3)))
			for j in range(len(data)):
				r[m][j] = data[j].split()
			m+=1

		else:
			end = True


	points = 0

	for m in range(len(r)):

		for j in range(3):
			l[m][0][j] = r[m][0][j]-r[m][-1][j]
		for i in range(1,len(r[m])):
			for j in range(3):
				l[m][i][j] = r[m][i][j]-r[m][i-1][j]



	for m in range(len(r)):

		points += len(r[m])

		for i in range(len(r[m])):
			rxeps[m][i] = np.cross(r[m][i], l[m][i])	
			for q in range(3):
				p[m][q] += rxeps[m][i][q]
				
	p *= kappa / 2	

	length = 0.0
	length_temp = 0.0
	for d in range(len(l)):
		for c in range(len(l[d])):
			for q in range(3):
				length_temp += pow(l[d][c][q],2)
			length += np.sqrt(length_temp)			

	return p, length, points

base_filename, files, times, jmax = getfiles()

kappa = 9.98e-8


impulse_files = []
impulse_times = []
impulse = []


for i in range(len(files)):
	if i%1==0:
		impulse_files.append(files[i])
		impulse_times.append(times[i])

length = np.zeros((len(impulse_times)))
points = np.zeros((len(impulse_times)))
p = np.zeros((jmax,3))
for i in range(len(impulse_files)):
	p, length[i], points[i] = calcimpulse(jmax, impulse_files, i)
	impulse.append(p)


fig = plt.figure(facecolor='w', edgecolor='w',figsize=plt.figaspect(2.))
ax = fig.add_subplot(111, 
 	axisbg='w')

ax2 = ax.twinx()
#ax3 = ax.twinx()


ax.set_xlim(0, np.max(impulse_times))
ax.set_ylim(0.0, 2*np.max(impulse))

p_total = np.zeros((len(impulse_times)))
p_total_z = np.zeros((len(impulse_times)))
p1 = np.zeros((len(impulse_times)))
p2 = np.zeros((len(impulse_times)))
p1_z = np.zeros((len(impulse_times)))
p2_z= np.zeros((len(impulse_times)))

for i in range(len(impulse)):
	p1[i] += np.sqrt(pow(impulse[i][0][0],2)+pow(impulse[i][0][1],2)+pow(impulse[i][0][2],2))
	p2[i] += np.sqrt(pow(impulse[i][1][0],2)+pow(impulse[i][1][1],2)+pow(impulse[i][1][2],2))
	p1_z[i] += impulse[i][0][2]
	p2_z[i] += impulse[i][1][2]
	for j in range(len(impulse[i])):
		p_total[i] += np.sqrt(pow(impulse[i][j][0],2)+pow(impulse[i][j][1],2)+pow(impulse[i][j][2],2))
		p_total_z[i] += impulse[i][j][2]



ax.set_ylabel('impulse')
ax.set_xlabel('time (s)')


ax2.set_ylim(0,1.05*np.max(length))
#ax3.set_ylim(0,1.05*np.max(points))

ring1 = ax.plot(impulse_times, p1, c='#9C2727',alpha=0.6, linewidth=3, label='ring 1 impulse')# s=35)
ring2 = ax.plot(impulse_times, p2, c='#1A5712',alpha=0.6, linewidth=3, label='ring 2 impulse')# s=35)
ring1_z = ax.plot(impulse_times, p1_z, c='r',alpha=0.7, linewidth=3, label='ring 1 z impulse')# s=35)
ring2_z = ax.plot(impulse_times, p2_z, c='g',alpha=0.7, linewidth=3, label='ring 2 z impulse')# s=35)
# ax.plot(impulse_times, p2, c='b',alpha=0.7, linewidth=3)# s=35)

ringtot = ax.plot(impulse_times, p_total, c='k',alpha=0.9,linewidth=3, label='total impulse')
ringtot = ax.plot(impulse_times, p_total_z, c='#363347',alpha=0.7,linewidth=3, label='total z impulse')
linetot = ax2.plot(impulse_times, length, c='c',linewidth=3, label = 'line length')
#ax3.plot(impulse_times, points, c='y', linewidth=3)
handles, labels = ax.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()

ax.legend(handles, labels)
ax2.legend(handles2, labels2, loc=2)

#ax2.legend([linetot], ['total line length'])
plt.show()



