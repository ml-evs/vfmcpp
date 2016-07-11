import os.path
import numpy as np
import matplotlib as matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import sys

#basest_filename = '../../data/offset_15R_hires/dat_-.8'
basest_filename = '../../../vfmcppar/data/multi_ring_test_4_core'
#basest_filename = '../../data/multi_ring_test_long'
#basest_filename = '../../data/kin_test_0'
#basest_filename = '../../data/kin_test_8'
#basest_filename = '../../data/kin_test_p4'
#basest_filename = '../../data/ring_ring_perp_norm'

kappa = 9.98e-8

def getfiles(basest_filename):
    files = list()
    times = list()
    k = 0
    j = 0
    jmax = 0

    #base_filename = '../../data/offset_15R/dat_-.9'
    
    base_filename = basest_filename + '/data_'
    end = False
    end2 = False
    f = 0
    while(end==False):
        end2 = False
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
            print ('File read successful. ')
    return base_filename, files, times, jmax
base_filename, files, times, jmax = getfiles(basest_filename)

fmax = len(files)
#fmax = 150
fstep = 1
f_ind = int(fmax / fstep)
i = 0
f = 0
fstart = 0
trajx = []
trajy = []
trajz = []
line_rx = []
line_ry = []
line_length = np.zeros((fmax))
ring_length = np.zeros((fmax))
ring_L_t = []

total_length = np.zeros((fmax))
tot_p_rings = np.zeros((fmax))
tot_px_rings = np.zeros((fmax))
tot_py_rings = np.zeros((fmax))
tot_pz_rings = np.zeros((fmax))
tot_p_line = np.zeros((fmax))
tot_px_line = np.zeros((fmax))
tot_py_line = np.zeros((fmax))
tot_pz_line = np.zeros((fmax))
total_p = np.zeros(fmax)
density = np.zeros((fmax))
N = np.zeros((fmax))
Ekin = np.zeros((fmax))
Ekin_line = np.zeros((fmax))
Ekin_ring = np.zeros((fmax))
while(f<fmax):
    end = False
    r = []
    L = []
    p = []
    p_line = []
    v = []
    m = 0
    fil = 0
    while(end == False):
        if os.path.isfile(files[f]+'_'+str(fil)+'.dat') == True:
            L.append(0)
            data = []
            file = open(files[f]+'_'+str(fil)+'.dat', 'r')
            data = file.readlines()
            data = np.delete(data,0)
            file.close()
            if(data[0] == data[-1]):
                r.append(np.zeros((3, len(data))))
                for j in range(len(data)):
                    for q in range(3):
                        r[m][q][j] = data[j].split()[q]
                for j in range(len(data)):
                    for q in range(3):
                        ring_length[i] += (r[m][q][j]-r[m][q][j-1])**2
                        L[m] += (r[m][q][j]-r[m][q][j-1])**2
                N[i] += len(data)
                L[m] = np.sqrt(L[m])

                if os.path.isfile(basest_filename+ '/mom_' + str(i) + '_' + str(fil)+'.dat') == True:
                    #print(basest_filename+ '/mom_' + str(i) + '_' + str(fil)+'.dat') 
                    momdata = []
                    #print(i)
                    momfile = open(basest_filename+ '/mom_' + str(i) + '_' + str(fil)+'.dat', 'r')
                    momdata = momfile.readlines()
                    momdata = np.delete(momdata,0)
                    momfile.close()
                    p = np.zeros((len(data),3))
                    for j in range(len(momdata)):
                        p[j] = momdata[j].split()
                    for j in range(len(momdata)):
                        tot_px_rings[i] += p[j][0]
                        tot_py_rings[i] += p[j][1]
                        tot_pz_rings[i] += p[j][2]

                m += 1
                fil += 1
               
            else:
                line_data = data
                line_points = np.zeros((len(line_data),3),dtype=float)
                for b in range(len(line_points)):
                    line_points[b] = line_data[b].split()
                j = len(line_points)-1
                while(j>0):
                    for q in range(3):
                        line_length[i] += (line_points[j][q]-line_points[j-1][q])**2
                    j -= 1
                N[i] += len(line_data)

                for datum in data:
                    line_rx.append(datum.split()[0])
                    line_ry.append(datum.split()[1])
                if os.path.isfile(basest_filename+ '/mom_' + str(i) + '_' + str(fil)+'.dat') == True:
                    momdata = []
                    momfile = open(basest_filename+ '/mom_' + str(i) + '_' + str(fil)+'.dat', 'r')
                    momdata = momfile.readlines()
                    momdata = np.delete(momdata,0)
                    momfile.close()
                    p_line = np.zeros((len(momdata),3))
                    for j in range(len(momdata)):
                        p_line[j] = momdata[j].split()
                    for j in range(len(momdata)):
                        tot_px_line[i] += p_line[j][0]
                        tot_py_line[i] += p_line[j][1]
                        tot_pz_line[i] += p_line[j][2]
                    tot_p_line[i] = np.sqrt(tot_px_line[i]**2 + tot_py_line[i]**2 + tot_pz_line[i]**2)

                fil += 1
        
        else:
            ring_L_t.append(L)
            tot_p_rings[i] = np.sqrt(tot_px_rings[i]**2 + tot_py_rings[i]**2 + tot_pz_rings[i]**2)
            ring_length[i] = np.sqrt(ring_length[i])
            line_length[i] = np.sqrt(line_length[i])
            total_length[i] = ring_length[i] + line_length[i]
            
            total_p[i] += (tot_px_rings[i] + tot_px_line[i])**2
            total_p[i] += (tot_py_rings[i] + tot_py_line[i])**2
            total_p[i] += (tot_pz_rings[i] + tot_pz_line[i])**2
            total_p[i] = np.sqrt(total_p[i])

            density[i] = total_length[i] / N[i]
            trajx.append(np.zeros((m)))
            trajy.append(np.zeros((m)))
            trajz.append(np.zeros((m)))
            for q in range(m):
                trajx[i][q] = np.mean(r[q][0][:])
                trajy[i][q] = np.mean(r[q][1][:])
                trajz[i][q] = np.mean(r[q][2][:])

            i += 1
            f += fstep
            end = True


if os.path.isfile(basest_filename+'/energy.dat') == True:
    energyfile = open(basest_filename+'/energy.dat')
    energydata = energyfile.readlines()
    energyfile.close()
    Ekin = np.zeros((f_ind,10))
    totEkin = np.zeros((f_ind))
    totEkin_smooth = np.zeros((f_ind))
    smooth = 16
    for i in range(f_ind):
        for j in range(len(energydata[i].split())):
            Ekin[i][j] = energydata[i].split()[j]
            totEkin[i] += Ekin[i][j]
            for k in range(i if i < smooth else smooth ):
                totEkin_smooth[i] += Ekin[i-k][j] / smooth

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(221,aspect='equal')
ax1 = fig.add_subplot(222)
ax2 = fig.add_subplot(223)
ax3 = fig.add_subplot(224)
#ax4 = fig.add_subplot(325)
#ax5 = fig.add_subplot(326)

colors = plt.cm.gnuplot(np.linspace(0,0.8,f_ind))
for a in range(len(trajx)):
    ax.scatter(trajy[a][:], trajx[a][:], lw=0, s=3, alpha=0.5, marker='o', c=colors[a])

colors2 = plt.cm.gnuplot_r(np.linspace(0.2,1,len(line_rx)))
line_rx = np.flipud(line_rx)
line_ry = np.flipud(line_ry)
ax.scatter(line_rx, line_ry, marker='o', s=2, alpha=0.2, lw=0.01, c=colors2)
ax.autoscale(enable=False)
ax.set_xlim(-6e-6,6e-6)
ax.set_ylim(-6e-6,6e-6)
ax.set_xticklabels([])
ax.set_yticklabels([])

temp1 = np.zeros((fmax))
temp2 = np.zeros((fmax))
temp3 = np.zeros((fmax))
for l in range(len(ring_L_t)):
    temp1[l] = ring_L_t[l][0]
    if(len(ring_L_t[l])>1):
        temp2[l] = ring_L_t[l][1]
    if(len(ring_L_t[l])>2):
        temp3[l] = ring_L_t[l][2]
# ax2.plot(temp1)
# ax2.plot(temp2)
# ax2.plot(temp3)
ax2.plot(line_length,marker='')
ax2.plot(ring_length,marker='')
ax2.plot(total_length,marker='',c='black')
ax2.set_xlim(fstart,f_ind-1)
ax2.set_ylim(0,1.2*np.max(total_length))

if os.path.isfile(basest_filename+'/energy.dat') == True:
    ax3.plot(Ekin,marker='',alpha=0.5,lw=0.5)
    ax3.set_xlim(smooth,len(Ekin))
    #ax3.plot(totEkin,marker='',lw=0.5,alpha=0.5, c='r')
    ax3.plot(totEkin_smooth,c='black',marker='',lw=1)

    #ax3.set_yticks([0,totEkin[0]])
    ax3.axhline(0,c='k')
if os.path.isfile(basest_filename+ '/mom_0_0.dat') == True:
    #ax1.plot(tot_px_rings,c='b')
    #ax1.plot(tot_py_rings,c='g')
    #ax1.plot(tot_pz_rings,c='r')
    #ax1.plot(tot_px_line,alpha=0.5,lw=0.5)
    #ax1.plot(tot_py_line,alpha=0.5,lw=0.5)
    #ax1.plot(tot_pz_line,alpha=0.5,lw=0.5)
    ax1.plot(tot_p_rings,alpha=0.5,lw=0.5)
    ax1.plot(tot_p_line,alpha=0.5,lw=0.5)
    ax1.plot(total_p, c='k',lw=1)
    #ax1.set_yticks([0,total_p[0]])
    #ax1.set_yticklabels(['0','1'])
    ax1.set_ylabel('Momentum $p/p_0$')
# d = np.zeros(fmax)
# for a in range(len(trajx)):
#     d[a] = trajx[a]**2 + trajy[a]**2 + trajz[a]**2
#     d[a] = np.sqrt(d[a])
# ax5.plot(np.abs(trajz),lw=0.5,alpha=0.5)
# ax5.plot(np.abs(trajx),lw=0.5,alpha=0.5)
# ax5.plot(np.abs(trajy),lw=0.5,alpha=0.5)
# ax5.plot(d,c='k')
## ax5.set_yticks([d[0]])
# ax5.set_yticklabels(['$d_0$'])
# ax5.set_ylabel('Displacement from $O$')

# ax6 = ax5.twinx()
# ax6.plot(N)

#ax2.set_yticks([0,total_length[0]])
#ax2.set_yticklabels(['0','1'])
#ax3.set_yticklabels(['0','1'])
ax2.set_ylabel('Vortex line length ($L/L_0$)')
ax3.set_ylabel('Kinetic energy ($E/E_0$)')
ax2.set_xlabel('t')
ax.axis('off')


print('Saving image...')
#plt.show()
plt.savefig(basest_filename + '/trajectory.png')
print('image saved.')