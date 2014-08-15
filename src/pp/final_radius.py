import sys
import os.path
import numpy as np
# import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

# pd.options.display.mpl_style = 'default'

# font = {'family' : 'monospace',
#         'size'   : 8}

# matplotlib.rc('font', **font)

files = list()
times = list()
sigma = list()
radius = list()
PLOT_DATA = list()

k = 0
j = 0
jmax = 0
base_filename = '../../data/sweep_09/data/'

# for root, dirs, files in os.walk(base_filename):
#   for name in dirs:
#   	sigma.append(name)
     



# for i in range(len(sigma)):
# 	end = False
# 	while(end==False):
# 		end2 = False		
# 		filename_k = base_filename + str(sigma[i]) + "/data_" + str(k)
# 		if os.path.isfile(filename_k+"_0.dat") == True:
# 			if os.path.isfile(base_filename+str(sigma[i])) + "/time.dat"
# 				last = filename_k	
# 		k+=1
# 		else:
# 			j = 0
# 			while(end2 == False):
# 				if os.path.isfile(last+"_" + str(j) + ".dat") == True:
# 					data = []
# 					r = []
# 					file = open(last+"_" + str(j) + ".dat")
# 					line = file.readline()
# 					line = file.readline()
# 					while line:
# 						data.append(line)
# 						line = file.readline()
# 					file.close()
# 					r   = np.zeros((len(data),3))
# 					l   = np.zeros((len(data),3))
# 					eps = np.zeros((len(data),3))
# 					rxeps = np.zeros((len(data),3))
# 					for j in range(len(data)):
# 						r[j] = data[j].split()

# 					for j in range(3):
# 						l[0][j] = r[0][j]-r[-1][j]
# 					for i in range(1,len(r)):
# 						for j in range(3):
# 							l[i][j] = r[i][j]-r[i-1][j]

# 					radius.append(0)
# 					for q in range(3):
# 						r_eff[q] = np.sqrt(pow(((1/(2*np.pi))*r_eff[q]),2))
# 						radius[-1] += r_eff[q]

# 					radius[-1] = np.sqrt(radius[-1])

# 				j+=1

# 				else:
# 					PLOT_DATA.append(sigma[i], radius)
# 					end2 = True

PAUL_DATA = []
data = []
file = open(base_filename+'Run3.dat')
line = file.readline()
while line:
	data.append(line)
	line = file.readline()
file.close()
for j in range(len(data)):
	PAUL_DATA.append(np.zeros((3)))
	PAUL_DATA[-1] = data[j].split()
	for i in range(3):
		PAUL_DATA[-1][i] = float(PAUL_DATA[-1][i])*1e-6 

fig = plt.figure(figsize=plt.figaspect(2), facecolor='w', edgecolor='w')
ax = fig.add_subplot(111, axisbg ='w')

PAUL_DATA = np.transpose(PAUL_DATA)

sig = PAUL_DATA[0]
r1 = PAUL_DATA[1]
r2 = PAUL_DATA[2]
ax.scatter(PAUL_DATA[0], PAUL_DATA[1], c='r')
ax.scatter(PAUL_DATA[0], PAUL_DATA[2], c='g')

ax.set_xlim(0,1e-6)
ax.set_ylim(0.0001e-6,1.2*max(r2))
plt.show()