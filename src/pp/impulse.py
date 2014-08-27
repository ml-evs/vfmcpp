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

	# T = []

	# A = []
	# B = []
	# C = []
	# D = []
	# E = []
	# sp = []


	# for m in range(len(r)):
	# 	A.append(np.zeros((len(r[m]))))
	# 	B.append(np.zeros((len(r[m]))))
	# 	C.append(np.zeros((len(r[m]))))
	# 	D.append(np.zeros((len(r[m]))))
	# 	E.append(np.zeros((len(r[m]))))
	# 	sp.append(np.zeros((len(r[m]),3)))
	# 	for i in range(len(r[m])):
	# 		j = i; k = i; l = i; q = i;
	# 		if j-2==-1:
	# 			j=len(r[m])+1
	# 		if j-2==-2:
	# 			j=len(r[m])
	# 		if k-1==-1:
	# 			k=len(r[m])
	# 		if l+1==len(r[m]):
	# 			l=-1
	# 		if q+1==len(r[q]):
	# 			q=-1
	# 		if q+2==len(r[q]):
	# 			q=-2
			
	# 		A[-1][i] = l[m][i]*l[m][l+1]*l[m][l+1]+l[m][i]*l[m][l+1]*l[m][q+2];
	# 		A[-1][i] = A[-1][i] / (l[m][k-1]*(l[m][k-1]+l[m][i])*(l[m][k-1]+l[m][i]+l[m][l+1])*(l[m][k-1]+l[m][i]+l[m][l+1]+l[m][q+2]));
			
	# 		B[-1][i] = -l[m][k-1]*l[m][l+1]*l[m][l+1] - l[m][i]*l[m][l+1]*l[m][l+1] - l[m][k-1]*l[m][l+1]*l[m][q+2] - l[m][i]*l[m][l+1]*l[m][q+2];
	# 		B[-1][i] = B[-1][i] / (l[m][k-1]*l[m][i]*(l[m][i]+l[m][l+1])*(l[m][i]+l[m][l+1]+l[m][q+2]));
	# 		D[-1][i] = l[m][k-1]*l[m][i]*l[m][l+1]+l[m][i]*l[m][i]*l[m][l+1]+l[m][k-1]*l[m][i]*l[m][q+2]+l[m][i]*l[m][i]*l[m][q+2];
	# 		D[-1][i] = D[-1][i] / (l[m][l+1]*l[m][q+2]*(l[m][i]+l[m][l+1])*(l[m][k-1]+l[m][i]+l[m][l+1]));
	# 		E[-1][i] = -l[m][l+1]*l[m][i]*l[m][i] - l[m][k-1]*l[m][i]*l[m][l+1];
	# 		E[-1][i] = E[-1][i] / (l[m][q+2]*(l[m][l+1] + l[m][q+2]) * (l[m][i]+l[m][l+1]+l[m][q+2])*(l[m][k-1]+l[m][i]+l[m][l+1]+l[m][q+2]));
	# 		C[-1][i] = -(A[-1][i] + B[-1][i] + D[-1][i] + E[-1][i]);	
		
	# 	for i in range(len(r[m]))
	# 		j = i; k = i; l = i; q = i;
	# 		if j-2==-1:
	# 			j=len(r[m])+1
	# 		if j-2==-2:
	# 			j=len(r[m])
	# 		if k-1==-1:
	# 			k=len(r[m])
	# 		if l+1==len(r[m]):
	# 			l=-1
	# 		if q+1==len(r[q]):
	# 			q=-1
	# 		if q+2==len(r[q]):
	# 			q=-2
			
	# 	for(int n=0;n<3;n++){
	# 		sp[-1][i][n] =  A[-1][i]*r[m][j-2][n]
	# 		sp[-1][i][n] += B[-1][i]*r[m][k-1][n]
	# 		sp[-1][i][n] += C[-1]*r[m][i][n]
	# 		sp[-1][i][n] += D[-1][i]*r[m][l+1][n]
	# 		sp[-1][i][n] += E[-1][i]*r[m][m+2][n]
	# 	}



	


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



PAUL_DATA = []
data = []

file = open('../../data/' +  str(sys.argv[1]) + '/paul.dat')
line = file.readline()
while line:
	data.append(line)
	line = file.readline()
file.close()
t = np.zeros((len(data)))
paul1 = np.zeros((len(data)))
paul2 = np.zeros((len(data)))
paul1z = np.zeros((len(data)))
paul2z = np.zeros((len(data)))
paul1xy = np.zeros((len(data)))
paul2xy = np.zeros((len(data)))
for j in range(len(data)):
	if float((data[j].split())[1]) < 0.0000001:
		t[j], paul1[j], paul2[j], paul1z[j], paul2z[j], paul1xy[j], paul2xy[j] = data[j].split()




fig = plt.figure(facecolor='w', edgecolor='w',figsize=((5,3)))
ax = fig.add_subplot(111, 
 	axisbg='w')

ax2 = ax.twinx()
#ax3 = ax.twinx()


ax.set_xlim(0, np.max(impulse_times))
ax.set_ylim(0.0, 1.2*np.max(impulse))

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


#ax2.set_ylim(0.95*np.min(length),1.05*np.max(length))
#ax3.set_ylim(0,1.05*np.max(points))

for i in range(len(impulse_files)):
	radius.append(0)
	radius[-1] = calcimpulse(jmax, impulse_files, i)
	erad[i] = (1.6e-19 * 10000 * impulse_times[i] /(3.14159265359*145*kappa)) + (np.min(radius)*np.min(radius))
	erad[i] = np.sqrt(erad[i])



ring1 = ax.plot(impulse_times, p1, c='#9C2727',alpha=0.9, linewidth=0.5, label='ring 1 impulse')# s=35)
ring2 = ax.plot(impulse_times, p2, c='r',alpha=0.9, linewidth=0.5, label='ring 2 impulse')# s=35)
ring1_z = ax.plot(impulse_times, p1_z, c='#9C2727',alpha=0.3, linewidth=0.5, label='ring 1 z impulse')# s=35)
ring2_z = ax.plot(impulse_times, p2_z, c='r',alpha=0.3, linewidth=0.5, label='ring 2 z impulse')# s=35)
paulplot1 = ax.plot(t, paul1, '-', markersize=3, c='#4A4DCF', alpha=0.2, linewidth=0.5, label='paul ring 1')
paulplot2 = ax.plot(t, paul2, '-', markersize=3, c='b', alpha=0.2, linewidth=0.5, label='paul ring 2')
paulplot1z = ax.plot(t, paul1z, '-', markersize=3, c='#4A4DCF', alpha=0.3, linewidth=0.5, label='paul ring 1 z')
paulplot2z = ax.plot(t, paul2z, '-', markersize=3, c='b', alpha=0.3, linewidth=0.5, label='paul ring 2 z')



# ax.plot(impulse_times, p2, c='b',alpha=0.7, linewidth=0.5)# s=35)

ringtot = ax.plot(impulse_times, p_total, c='k',alpha=0.9,linewidth=0.5, label='total impulse')
ringtot = ax.plot(impulse_times, p_total_z, c='#363347',alpha=0.7,linewidth=0.5, label='total z impulse')
#linetot = ax2.plot(impulse_times, length, c='c',linewidth=0.5, label = 'line length')
#ax3.plot(impulse_times, points, c='y', linewidth=0.5)
handles, labels = ax.get_legend_handles_labels()
#handles2, labels2 = ax2.get_legend_handles_labels()




ax.legend(handles, labels, prop={'size':4}, loc=2)
#ax2.legend(handles2, labels2, loc=2)

#ax2.legend([linetot], ['total line length'])
#plt.show()

fig.savefig('../../data/' + str(sys.argv[1]) + '/impulse.png', dpi=400, facecolor='w', edgecolor='w',
         orientation='portrait', pad_inchemarkersize=0.1)

