import sys
import os.path
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

#pd.options.display.mpl_style = 'default'

font = {'family' : 'serif',
        'size'   : 8}

matplotlib.rc('font', **font)

def getfiles():
	files = list()
	times = list()
	k = 0
	j = 0
	jmax = 0
	base_filename = '../../data/test_charge/data_'
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
radius = []


for i in range(len(files)):
	if i%1==0:
		impulse_files.append(files[i])
		impulse_times.append(times[i])

length = np.zeros((len(impulse_times)))
points = np.zeros((len(impulse_times)))
p = np.zeros((jmax,3))
for i in range(len(impulse_files)):
	p, length[i], points[i] = calcimpulse(1, impulse_files, i)
	impulse.append(p)
	r = np.sqrt(np.sqrt(p[0][0]*p[0][0]+p[0][1]*p[0][1]+p[0][2]*p[0][2])/(np.pi*kappa))
	radius.append(r)


erad = np.zeros((len(impulse_files)))
for i in range(len(impulse_files)):
	erad[i] = (1.6e-19 * 10000 * impulse_times[i] /(3.14159265359*145*kappa)) + (np.min(radius)*np.min(radius))
	erad[i] = np.sqrt(erad[i])

min_r = np.min(radius)

fig = plt.figure(facecolor='w', edgecolor='w',figsize=((5,3)))
ax = fig.add_subplot(111, 
 	axisbg='w')

#ax2 = ax.twinx()
#ax3 = ax.twinx()


ax.set_xlim(0, 0.001)
ax.set_ylim(0.95e-6, 1.015e-6)
ax.set_yticks([min_r])
ax.set_yticklabels('r-init')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Effective radius (m)')



ax.set_ylabel('impulse')
ax.set_xlabel('time (s)')


#ax2.set_ylim(0.95*np.min(length),1.05*np.max(length))
#ax3.set_ylim(0,1.05*np.max(points))

ax.plot(impulse_times, radius, c='r',alpha=0.7, linewidth=1)# s=35)
ax.plot(impulse_times, erad, c='g',alpha=0.7, linewidth=1)


plt.show()

# fig.savefig('../../data/' + str(sys.argv[1]) + '/impulse.png', dpi=400, facecolor='w', edgecolor='w',
#          orientation='portrait', pad_inchemarkersize=0.1)

