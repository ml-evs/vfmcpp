import sys
import os.path
import numpy as np
#import pandas as pd
import matplotlib as matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#pd.options.display.mpl_style = 'default'

font = {'family' : 'Serif',
				'serif' : 'Times New Roman',
        'size'   : 16}


matplotlib.rc('font', **font)

files = list()
times = list()
sigma = list()
no_recon = list()
PLOT_DATA = list()
ZERO_DATA = list()
THREE_DATA = list()
ZERO_DATA_PAUL = list()
FAILED_DATA = list()

base_filename = '../../data/01_sweep/'

for root, dirs, files in os.walk(base_filename):
  for name in dirs:
  	if name[0] != 's':
   		sigma.append(name)
   		##print name
     
kappa = 9.98e-8


for i in range(len(sigma)):
	last = ''
	end = False
	#print sigma[i]
	k = 0
	while(end==False):
		end2 = False		
		filename_k = base_filename + str(sigma[i]) + "/data_" + str(k)
		if os.path.isfile(base_filename + str(sigma[i]) + "/time.dat") == False:
			FAILED_DATA.append([float(sigma[i][0:6])*1e-6, 1.7e-6])
			end = True
		elif os.path.isfile(filename_k+"_0.dat") == True:
			last = filename_k
			k+=2
			continue
		else:
			j = 0
			end = True
			radius = []
			impulse = np.zeros((3))
			while(end2 == False):
				if os.path.isfile(last+"_" + str(j) + ".dat") == True:
					data = []
					file = open(last+"_" + str(j) + ".dat")
					line = file.readline()
					line = file.readline()
					while line:
						data.append(line)
						line = file.readline()
					file.close()
					r   = np.zeros((len(data),3))
					r_eff = np.zeros((3))
					l   = np.zeros((len(data),3))
					eps = np.zeros((len(data),3))
					rxeps = np.zeros((len(data),3))
					for b in range(len(data)):
						r[b] = data[b].split()

					for b in range(3):
						l[0][b] = r[0][b]-r[-1][b]
					for q in range(1,len(r)):
						for b in range(3):
							l[q][b] = r[q][b]-r[q-1][b]

					for b in range(len(r)):
						for q in range(3):
							eps[b][q] = l[b][q]
					for b in range(len(r)):
						rxeps[b] = np.cross(r[b], eps[b])	
						for q in range(3):
							impulse[q] += rxeps[b][q]
				
					impulse *= kappa / 2 
					total_impulse = np.sqrt(pow(impulse[0],2) + pow(impulse[1],2) + pow(impulse[2],2))
					radius.append(0)
					radius[-1] = np.sqrt(total_impulse / (np.pi * kappa))
					#print radius
					j+=1
				

				else:
					##print i
					print len(radius), sigma[i]
					if len(radius)==1:
						ZERO_DATA.append([float(sigma[i][0:6])*1e-6, radius[0]])
						#print sigma[i], radius[0]
					elif len(radius) == 2:
						file = open(base_filename + str(sigma[i]) + "/time.dat")
						line = file.readline()
						line = file.readline()
						no_recon.append(line[-2])
						print no_recon[-1]
						file.close()
						#print sigma[i], radius[0], radius[1]
						PLOT_DATA.append([float(sigma[i][0:6])*1e-6, radius[0], radius[1]])
					elif len(radius) == 3:
						THREE_DATA.append([float(sigma[i][0:6])*1e-6, radius[0], radius[1], radius[2]])
					elif len(radius) > 3:
						print '4 rings'
					end = True
					end2 = True
	#print i

fig = plt.figure(figsize=(5,5), facecolor='w', edgecolor='w')
ax = fig.add_subplot(111, axisbg ='w')
#ax2 = fig.add_subplot(212, axisbg='w',sharex=ax)

#plt.subplots_adjust(left=None, bottom=0.1, right=None, top=None, wspace=None, hspace=0.1)

BIG_DATA = []
LITTLE_DATA = []
for i in range(len(PLOT_DATA)):
	BIG_DATA.append(max(PLOT_DATA[i][1],PLOT_DATA[i][2]))
	LITTLE_DATA.append(min(PLOT_DATA[i][1],PLOT_DATA[i][2]))

PLOT_DATA = np.transpose(PLOT_DATA)

ALL_DATA = np.asarray([PLOT_DATA[0],LITTLE_DATA, BIG_DATA])
ALL_DATA = np.transpose(ALL_DATA)
ALL_DATA = np.sort(ALL_DATA, axis=0)

if(len(ZERO_DATA)!=0):
	ZERO_DATA = np.transpose(ZERO_DATA)

FAILED_DATA = np.transpose(FAILED_DATA)

BIG_DATA = np.transpose(BIG_DATA)
LITTLE_DATA = np.transpose(LITTLE_DATA)
THREE_DATA = np.transpose(THREE_DATA)
ALL_DATA = np.transpose(ALL_DATA)

c = []
for i in range(len(no_recon)):
	if no_recon[i] == '0':
		c.append('g')
	if no_recon[i] == '1':
		c.append('b')
	if no_recon[i] >= '2':
		c.append('b')



ax.vlines(PLOT_DATA[0], 0, 3e-6, linewidth=10, alpha=0.15, color=c)
if(len(ZERO_DATA)!=0):
	ax.vlines(ZERO_DATA[0], 0, 3e-6, linewidth=10, color='r', alpha=0.15)

if(len(THREE_DATA)!=0):
	ax.vlines(THREE_DATA[0], 0, 3e-6, linewidth=10, color='y', alpha=0.35)
	ax.plot(THREE_DATA[0],THREE_DATA[1], linewidth=0, c='b', marker='o', markerfacecolor='k', markeredgecolor='k', markersize=5, alpha=1)
	ax.plot(THREE_DATA[0],THREE_DATA[2], linewidth=0, c='b', marker='o', markerfacecolor='k', markeredgecolor='k', markersize=5, alpha=1)
	ax.plot(THREE_DATA[0],THREE_DATA[3], linewidth=0, c='b', marker='o', markerfacecolor='k', markeredgecolor='k', markersize=5, alpha=1)

ax.vlines(PLOT_DATA[0], PLOT_DATA[1], PLOT_DATA[2], linewidth=0.8, alpha=0.8, color=c)


r1 = ax.plot(PLOT_DATA[0], BIG_DATA, linewidth=0, c='b', marker='^', markerfacecolor='k', markeredgecolor='k', markersize=5, alpha=1)
r2 = ax.plot(PLOT_DATA[0], LITTLE_DATA, linewidth=0, c='b', marker='v', markerfacecolor='k', markeredgecolor='k', markersize=5, alpha=1)

if(len(ZERO_DATA)!=0):
	r0 = ax.plot(ZERO_DATA[0], ZERO_DATA[1], linewidth=0, c='w', markersize=8, markerfacecolor='k', marker='*', alpha=1)

ax.set_xticks([0,0.09e-6,0.2e-6,0.25e-6,0.5e-6])
ax.set_xticklabels([0,0.09,0.20,0.25,0.5])
ax.set_yticks([0,0.9e-6,0.7e-6,0.8e-6,1e-6,1.1e-6,1.2e-6])
ax.set_yticklabels([0,0.9,0.7,0.8,1,1.1,1.2])
ax.set_ylabel('Effective radius (um)')


ax.set_xlim(0,5.5e-7)
ax.set_ylim(0.000,1.5e-6)

ax.set_xlabel('Impact parameter (um)')

#plt.show()
fig.savefig(base_filename+'../../img/impact_vs_r_eff_01.png')
