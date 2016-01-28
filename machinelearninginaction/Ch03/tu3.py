import sys
from pylab import *
from numpy import *
import numpy as np
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

'''
[[1, 1, 'yes'],
[1, 1, 'yes'],
[1, 0, 'no'],
[0, 1, 'no'],
[0, 1, 'no']]
'''

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
ax.scatter(1, 2, 1, 'r', marker='o')
ax.scatter(1, 1, 1, 'r', marker='o')
ax.scatter(1, 0, 0, 'r', marker='o')
ax.scatter(0, 1, 0, 'r', marker='o')
ax.scatter(0, 1, 0, 'r', marker='o')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()
