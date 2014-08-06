import os.path
import numpy as np
import sys
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
import pandas as pd
import matplotlib

pd.options.display.mpl_style = 'default'

font = {'family' : 'monospace',
        'size'   : 8}

matplotlib.rc('font', **font)

class Line():
	def __init__(self):
		self.L = []
		self.dr = []

	def linelength(self, r):
		l = []
		for j in range (len(r)):
			l.append(0)
			for k in range(len(r[j])-1):
				temp = []
				for q in range(3):
					temp.append(pow(r[j][k][q]-r[j][k+1][q],2))
				tempsum = np.sqrt(sum(temp))
				l[j] += tempsum
		self.L.append(sum(l))

	def res(self,r):
		l = []
		no_points = 0
		for j in range (len(r)):
			l.append(0)
			for k in range(len(r[j])-1):
				no_points +=1
				temp = []
				for q in range(3):
					temp.append(pow(r[j][k][q]-r[j][k+1][q],2))
				l[j] += np.sqrt(sum(temp))
		self.dr.append((4.0/(3.0*no_points)) * sum(l))

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

