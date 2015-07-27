import sys
import os.path
import numpy as np
import matplotlib as matplotlib
import matplotlib.animation as animation
#print(matplotlib.matplotlib_fname())
#matplotlib.use('Agg')
kappa = 9.98e-8
import matplotlib.pyplot as plt

sigma_list = list()
base_filename = '../../data/kin_test_p4'

def init():
    for line in lines:
        line.set_data([],[])
    return lines


def animate(i):
    end = False
    J = 0
    while(end == False):
        if os.path.isfile(base_filename+'/data_' + str(i) + '_' + str(J) + '.dat') == True:
            data1 = []
            datafile1 = open(base_filename+'/data_' + str(i) + '_' + str(J) + '.dat', 'r')
            data1 = datafile1.readlines()
            data1 = np.delete(data1,0)
            if(data1[0] == data1[-1]):
                J += 1
            else:
                #print(i)
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

    lines[0].set_data(freq, np.real(ftx))
    lines[1].set_data(freq, np.real(fty))
    lines[2].set_data(freq, power)
    fig.canvas.draw()
    plt.draw()
    return lines


fig = plt.figure(figsize=(3,6))
ax = fig.add_subplot(311)
ax2 = fig.add_subplot(312)
ax3 = fig.add_subplot(313)
#time_text = ax.text(0, 0, 0,'', color='k')
# for k in range (jmax+2):
#     rings += [l for c in colors for l in ax.plot([], [], [], style, c=np.random.rand(3,1), alpha = 0.9, linewidth=2, markersize=5, markerfacecolor=c, markeredgecolor=c)]
lines = []
lines += [l for l in ax.plot([], [])]
lines += [l for l in ax2.plot([], [])]
lines += [l for l in ax3.plot([], [])]


ani = animation.FuncAnimation(fig, animate, repeat=False, frames = 250, interval = 1, blit=False)

ax.set_ylim(-1e-4,1e-4)
ax2.set_ylim(-1e-4,1e-4)
ax3.set_ylim(-1e-4,1e-4)

ax.set_xlim(0,0.4)
ax2.set_xlim(0,0.4)
ax3.set_xlim(0,0.4)
# ax.plot(freq, np.real(ftx), freq, np.imag(ftx))
# ax2.plot(freq, np.real(fty), freq, np.imag(fty))
# ax3.plot(freq, np.abs(ftx+fty))
plt.show()
