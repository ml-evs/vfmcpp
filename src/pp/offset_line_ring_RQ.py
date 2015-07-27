import sys
import os.path
import numpy as np
import matplotlib as matplotlib
#print(matplotlib.matplotlib_fname())
matplotlib.use('Agg')
kappa = 9.98e-8
import matplotlib.pyplot as plt

sigma_list = list()
base_filename = '../../data/offset_15R/'
for root, dirs, files in os.walk(base_filename):
    for name in dirs:
        if name[0] != 's':
            #if os.path.isfile(base_filename + name + '/time.dat') == True:
            if 0==0:
                sigma_list.append(name)

circumference = np.zeros(len(sigma_list),dtype=np.float64)
impulse_rings = np.zeros(len(sigma_list),dtype=np.float64)
line_length = np.zeros(len(sigma_list),dtype=np.float64)
total_impulse = np.zeros(len(sigma_list),dtype=np.float64)
index = 0
done_list = np.zeros(len(sigma_list))
for sigma in sigma_list:
    if os.path.isfile(base_filename+sigma+'/events.log') == True:
        lastdat = 0
        for root, dirs, files in os.walk(base_filename+sigma):
            for name in files:
                if(len(name.split('_'))!=1):
                    int_dat = int(name.split('_')[1])
                if int_dat > lastdat:
                    lastdat = int_dat
        lastdat = 300
        log = []
        logfile = open(base_filename+sigma+'/events.log', 'r')
        log = logfile.readlines()
        log = log[-1].split('\t')
        if(log[0:14]!='number of loop'):
            done_list[index] = 0.8
        else:
            done_list[index] = 0.2

        end = False
        J = 0
        while(end == False):
            if os.path.isfile(base_filename+sigma+'/data_' + str(lastdat) + '_' + str(J) + '.dat') == True:
                data1 = []
                datafile1 = open(base_filename+sigma+'/data_' + str(lastdat) + '_' + str(J) + '.dat', 'r')
                data1 = datafile1.readlines()
                data1 = np.delete(data1,0)
                if(data1[0] == data1[-1]):
                    mesh_segs = np.zeros((len(data1),3),dtype=float)
                    points = np.zeros((len(data1),3),dtype=float)        
                    rxl = np.zeros((len(data1),3),dtype=float)        
                    com = [np.mean(points[:,0]),np.mean(points[:,1]), np.mean(points[:,2])]
                    for b in range(len(points)):
                        circumference[index] += np.sqrt(np.sum((points[b]-points[b-1])**2))

                    rad_C[index] = circumference[index] / (2*np.pi)
                    if os.path.isfile(base_filename+sigma+'/mom_' + str(i) + '_' + str(fil)+'.dat') == True:
                        momdata = []
                        momfile = open(base_filename+sigma+'/mom_' + str(i) + '_' + str(fil)+'.dat', 'r')
                        momdata = momfile.readlines()
                        momdata = np.delete(momdata,0)
                        momfile.close()
                        p = np.zeros((len(data),3))
                        for j in range(len(momdata)):
                            p[j] = momdata[j].split()
                        for j in range(len(momdata)):
                            tot_px_rings += p[j][0]
                            tot_py_rings += p[j][1]
                            tot_pz_rings += p[j][2]
                        impulse_rings[index] += np.sqrt(tot_px_rings**2 + tot_py_rings**2 + tot_pz_rings**2)
                else:
                    line_data = data1
                    line_points = np.zeros((len(line_data),3),dtype=float)
                    for b in range(len(line_data)):
                        line_points[b] = line_data[b].split()
                    i = len(line_points)-1
                    while(i>0):
                        line_length[index] += np.sqrt(np.sum((line_points[i]-line_points[i-1])**2))
                        i -= 1
                J+=1
            else:
                end = True
        
    index += 1

sigma_array = np.zeros(len(sigma_list), dtype=np.float64)
i = 0
for sigma in sigma_list:
    sigma_array[i] = sigma.split('_')[1]
    i += 1

inds = sigma_array.argsort()
rad_C = rad_C[inds]
circumference = circumference[inds]
mean_radius = mean_radius[inds]
line_length = line_length[inds]
sigma_array = sigma_array[inds]
done_list = done_list[inds]

fig = plt.figure(figsize=(5,5))

ax = fig.add_subplot(111)
#ax2 = ax.twinx()
ax.plot(-sigma_array, 1e6*(line_length+circumference),marker='o',color='black', alpha=0.8)
ax.plot( -sigma_array, 1e6*circumference,    marker='o',   color='blue', alpha=0.8)
ax.plot(-sigma_array, 1e6*line_length, marker='o',color='red', alpha=0.8)
ax.plot(-sigma_array, 1e6*2*np.pi*np.sqrt(impulse_rings/(2*np.pi)), color='red', alpha=0.8)

p = plt.axvspan(-2, -0.7, facecolor='0.25', alpha=0.5)
p = plt.axvspan(1.45, 2, facecolor='0.25', alpha=0.5)
ax.set_xlabel('Impact parameter ($\mu\\mathrm{m}$)')
ax.set_ylabel('Line length ($\mu\\mathrm{m}$)')
ax.set_xlim(-2,2)

#ax.set_ylim(0,1.5*np.max(rad_C))
ax.set_ylim(-0.2,25)
print('Saving image...')
plt.savefig(base_filename+'analysis.png')
print('image saved.')

