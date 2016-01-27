import sys
import kNN
from pylab import *
from numpy import *
import numpy as np
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
mat,lab = kNN.file2matrix('datingTestSet2.txt')
normMat, ranges, minVals = kNN.autoNorm(mat)

def randrange(n, vmin, vmax):
    return (vmax - vmin)*np.random.rand(n) + vmin

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
#ax.scatter(normMat[:,0], normMat[:,1], normMat[:,2], 'o', 'c')
n = 1
for c, m, zl, zh in [('r', 'o', -50, -25), ('b', '^', -30, -5)]:
    xs = randrange(n, 23, 32)
    ys = randrange(n, 0, 100)
    zs = randrange(n, zl, zh)

ClassSet=lab
colorSet = []
for label in ClassSet:
	if label is '1':
		colorSet.append('r')
	elif label is '2':
		colorSet.append('b')
	elif label is '3':
		colorSet.append('y')
	else:
		colorSet.append('r')
print(colorSet)

x=normMat[:,0]
y=normMat[:,1]
z=normMat[:,2]
i=0
for lx in x:
	ly = y[i]
	lz = z[i]
	lc = colorSet[i]
	i=i+1
	print(lx,ly,lz,lc)
	ax.scatter(lx, ly, lz, c=lc, marker='o')
#ax.scatter(normMat[:,0], normMat[:,1], normMat[:,2], colorSet, marker='o')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()
