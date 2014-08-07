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
	end = False
	m=0
	r = []
	sp = []
	l= []
	eps = []
	rxeps = []
	r_eff = []
	radius = np.zeros((jmax+1))
	A = []
	B = []
	C = []
	D = []
	E = []

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
			r_eff.append(np.zeros((3)))
			l.append(np.zeros((len(data),3)))
			eps.append(np.zeros((len(data),3)))
			rxeps.append(np.zeros((len(data),3)))
			A.append(np.zeros((len(data))))
			B.append(np.zeros((len(data))))
			C.append(np.zeros((len(data))))
			D.append(np.zeros((len(data))))
			E.append(np.zeros((len(data))))
			for j in range(len(data)):
				r[m][j] = data[j].split()
			#print m
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
			for q in range(3):
				eps[m][i][q] = l[m][i][q]

		for i in range(len(r[m])):
			rxeps[m][i] = np.cross(r[m][i], eps[m][i])	
			for q in range(3):
				r_eff[m][q] += rxeps[m][i][q]
				
			

		for q in range(3):
			r_eff[m][q] = np.sqrt(pow(((1/(2*np.pi))*r_eff[m][q]),2))
			radius[m] += r_eff[m][q]

		radius[m] = np.sqrt(radius[m])

	for b in range(len(r),jmax+1):
		r[m] = np.nan

	length = 0.0
	length_temp = 0.0
	for d in range(len(l)):
		for c in range(len(l[d])):
			for q in range(3):
				length_temp += pow(l[d][c][q],2)
			length += np.sqrt(length_temp)			

	return radius, length, points

base_filename, files, times, jmax = getfiles()

kappa = 9.98e-8


impulse_files = []
impulse_times = []
radius = []


for i in range(len(files)):
	if i%1==0:
		impulse_files.append(files[i])
		impulse_times.append(times[i])

length = np.zeros((len(impulse_times)))
points = np.zeros((len(impulse_times)))
for i in range(len(impulse_files)):
	radius.append(np.zeros((jmax+1)))
	radius[-1], length[i], points[i] = calcimpulse(jmax, impulse_files, i)

fig = plt.figure(facecolor='w', edgecolor='w',figsize=plt.figaspect(2.))
ax = fig.add_subplot(111, 
 	axisbg='w')

ax2 = ax.twinx()
ax3 = ax.twinx()


impulse = np.pi * kappa * np.power(radius,2)

ax.set_xlim(0, np.max(impulse_times))
ax.set_ylim(0.000001*np.max(impulse), 1.5*np.max(impulse))

p_total = np.zeros((len(impulse_times)))

for i in range(len(radius)):
	for j in range(len(radius[i])):
		p_total[i] += impulse[i][j]
		if impulse[i][j] == 0:
			impulse[i][j] = np.nan




p_mask = np.ma.array(impulse)
r_mask = np.ma.array(radius)

p0 = p_mask[:,0]
p1 = p_mask[:,1]
p2 = p_mask[:,2]


ax2.set_ylim(0,1.2*np.max(length))
ax3.set_ylim(0,1.2*np.max(points))

print radius[0]

ax.plot(impulse_times, p0, c='r',alpha=0.7, linewidth=3)# s=35)
ax.plot(impulse_times, p1, c='g',alpha=0.7, linewidth=3)# s=35)
ax.plot(impulse_times, p2, c='b',alpha=0.7, linewidth=3)# s=35)
ax.plot(impulse_times, p_total, c='k',alpha=0.9,linewidth=3)
ax2.plot(impulse_times, length, c='c',linewidth=3)
ax3.plot(impulse_times, points, c='y', linewidth=3)
plt.show()




