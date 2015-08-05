import sys
import os.path
import numpy as np
import matplotlib as matplotlib
#print(matplotlib.matplotlib_fname())
matplotlib.use('Agg')
kappa = 9.98e-8
import matplotlib.pyplot as plt

sigma_list = list()
root_list = ['../../data/offset_15R_dist_t0/', '../../data/offset_15R_dist_t4/', '../../data/offset_15R_dist_t8/']
base_filename = '../../data/offset_15R_dist_t0/'
for base_filename in root_list:
    for root, dirs, files in os.walk(base_filename):
        for name in dirs:
            if name[0] != 's':
                #if os.path.isfile(base_filename + name + '/time.dat') == True:
                if 0==0:
                    sigma_list.append(base_filename + name)



circumference = np.zeros(len(sigma_list),dtype=np.float64)
impulse_rings = np.zeros(len(sigma_list),dtype=np.float64)
line_length = np.zeros(len(sigma_list),dtype=np.float64)
total_impulse = np.zeros(len(sigma_list),dtype=np.float64)
tot_px_rings = 0.0
tot_py_rings = 0.0
tot_pz_rings = 0.0
index = 0
killed = np.zeros(len(sigma_list))
for sigma in sigma_list:
    if os.path.isfile(sigma+'/events.log') == True:
        lastdat = 0
        for root, dirs, files in os.walk(sigma):
            for name in files:
                if(len(name.split('_'))!=1):
                    int_dat = int(name.split('_')[1])
                if int_dat > lastdat:
                    lastdat = int_dat
        #lastdat = 600
        log = []
        logfile = open(sigma+'/events.log', 'r')
        log = logfile.readlines()
        log = log[-1].split('\t')
        print(sigma)
        if(log[0:14]!='number of loop'):
            killed[index] = int(log[-1].split(' ')[-1])
        end = False
        J = 0
        while(end == False):
            if os.path.isfile(sigma+'/data_' + str(lastdat) + '_' + str(J) + '.dat') == True:
                data = []
                datafile = open(sigma+'/data_' + str(lastdat) + '_' + str(J) + '.dat', 'r')
                data = datafile.readlines()
                data = np.delete(data,0)
                if(data[0] == data[-1]):
                    points = np.zeros((len(data),3),dtype=float)        
                    for b in range(len(data)):
                        points[b] = data[b].split()
                    for b in range(len(points)):
                        circumference[index] += np.sqrt(np.sum((points[b]-points[b-1])**2))

                    if os.path.isfile(sigma+'/mom_' + str(lastdat) + '_' + str(J)+'.dat') == True:
                        momdata = []
                        momfile = open(sigma+'/mom_' + str(lastdat) + '_' + str(J)+'.dat', 'r')
                        momdata = momfile.readlines()
                        momdata = np.delete(momdata,0)
                        momfile.close()
                        p = np.zeros((len(momdata),3))
                        for j in range(len(momdata)):
                            p[j] = momdata[j].split()
                            tot_px_rings += p[j][0]
                            tot_py_rings += p[j][1]
                            tot_pz_rings += p[j][2]

                else:
                    line_data = data
                    line_points = np.zeros((len(line_data),3),dtype=float)
                    for b in range(len(line_data)):
                        line_points[b] = line_data[b].split()
                    i = len(line_points)-1
                    while(i>0):
                        line_length[index] += np.sqrt(np.sum((line_points[i]-line_points[i-1])**2))
                        i -= 1
                J+=1

            else:
                impulse_rings[index] += np.sqrt(tot_px_rings**2 + tot_py_rings**2 + tot_pz_rings**2)
                end = True
    index += 1
    tot_px_rings = 0.0
    tot_py_rings = 0.0
    tot_pz_rings = 0.0

sigma_array = np.zeros(len(sigma_list), dtype=np.float64)
i = 0
for sigma in sigma_list:
    sigma_array[i] = sigma.split('_')[-1]
    i += 1

inds = sigma_array.argsort()
circumference = circumference[inds]
impulse_rings = impulse_rings[inds]
line_length = line_length[inds]
sigma_array = sigma_array[inds]
killed = killed[inds]

fig = plt.figure(figsize=(10,8))
ax = fig.add_subplot(111)
p = plt.axvspan(-2, -0.7, facecolor='y', alpha=0.15)
p = plt.axvspan(1.44, 2, facecolor='y', alpha=0.15)
p = plt.axvspan(1.00, 1.44, facecolor='b', alpha=0.15)
p = plt.axvspan(0.56, 1.00, facecolor='r', alpha=0.15)
p = plt.axvspan(-0.7, 0.56, facecolor='g', alpha=0.15)

sigma_array_slice = np.linspace(-1,1,25)
sigma_array_slice *= 1e-6
theory = 2*np.sqrt((1e-6**2 - sigma_array_slice**2)) + 2*1e-6*np.arccos(sigma_array_slice/1e-6)

ax.scatter(-sigma_array, 1e6*(line_length+circumference),color='black', alpha=1,label='total length', lw=1.5, marker='o')
ax.scatter(-sigma_array, 1e6*circumference,       color='blue', alpha=1, label='length around rings', lw=1.5, marker='o')
ax.scatter(-sigma_array, 1e6*line_length, color='red', alpha=1, label='length of line', lw=1.5, marker='o')
ax.scatter(-sigma_array, 1e6*np.sqrt(2*np.pi*impulse_rings), color='green', alpha=1, label='effective circumference of rings', lw=1.5, marker='o')
ax.plot(1e6*sigma_array_slice, 1e6*theory, linestyle='--', c='magenta',label='geometric reconnection', lw=1.5)
from matplotlib.font_manager import FontProperties
fontP = FontProperties()
fontP.set_size('small')
ax.legend(prop = fontP,loc=2)
ax.set_xlabel('Impact parameter ($\mu\\mathrm{m}$)')
ax.set_ylabel('Line length ($\mu\\mathrm{m}$)')
ax.set_xlim(-2,2)
#ax.set_yticks([1e6*(line_length[0]+circumference[0]),1e6*line_length[0], 1e6*circumference[0], 0.5e6*(line_length[0]+circumference[0])])
#ax.set_yticklabels(['$L_0$', '$l_0$', '$2\pi r_0$', '$L_0/2$'])
# ax2 = ax.twinx()
# ax2.set_yticks([1e6*(line_length[0]+circumference[0]),1e6*line_length[0], 1e6*circumference[0], 0.5e6*(line_length[0]+circumference[0])])
# ax2.set_yticklabels(['$L_0$', '$l_0$', '$2\pi r_0$', '$L_0/2$'])
ax.set_ylim(0,1.5e6*(line_length[0]+circumference[0]))
# ax2.set_ylim(0,26)

ax3 = ax.twinx()
ax3.bar(-sigma_array-0.02, killed, 0.04, color='k', alpha=0.2)
ax3.axhline(2,alpha=0.2,color='k')
ax3.set_ylim(-18, np.max(killed)+30)
ax3.set_yticks([0, 2,np.max(killed)])
ax3.set_ylabel('Number of small loops killed')
ax3.set_xlim(-2,2)
print('Saving image '+root_list[0]+'analysis.png.')
plt.savefig(root_list[0]+'analysis.png')
print('image saved.')

