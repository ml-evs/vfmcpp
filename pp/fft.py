import sys
import os.path
import numpy as np
import matplotlib as matplotlib
import matplotlib.animation as animation
#print(matplotlib.matplotlib_fname())
matplotlib.use('Agg')
kappa = 9.98e-8
import matplotlib.pyplot as plt

base_filename = '../../data/offset_15R/dat_0'

end = False
J = 0
i = 220

while(end == False):
    if os.path.isfile(base_filename+'/data_' + str(i) + '_' + str(J) + '.dat') == True:
        data1 = []
        datafile1 = open(base_filename+'/data_' + str(i) + '_' + str(J) + '.dat', 'r')
        data1 = datafile1.readlines()
        data1 = np.delete(data1,0)
        if(data1[0] == data1[-1]):
            J += 1
        else:
            line_data = data1
            line_points = np.zeros((len(line_data),3),dtype=float)
            for q in range(len(data1)):
                line_points[q] = data1[q].split()
            line_points = line_points.T
            ftx = np.fft.fft(line_points[0])
            fty = np.fft.fft(line_points[1])
            freq = np.fft.fftfreq(line_points[2].shape[-1])
            power = np.abs(ftx+fty)
            end = True

fig = plt.figure(figsize=(4,4))
ax = fig.add_subplot(111)

ax.set_ylim(-1e-4,1e-4)
ax.set_yticks([])

ax.set_xlim(-0.2,0.2)

#ax.plot(freq, np.real(ftx), freq, np.imag(ftx), c='b')
#ax.plot(freq, np.real(fty), freq, np.imag(fty), c='r')
ax.plot(freq, np.abs(ftx+fty), c ='k')
plt.savefig('fft.png')
