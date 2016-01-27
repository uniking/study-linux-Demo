import sys
import kNN
from numpy import *
import matplotlib
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(111)
mat,lab = kNN.file2matrix('datingTestSet2.txt')
ax.scatter(mat[:,1], mat[:,2], 15.0*array(map(int, lab)), 15.0*array(map(int,lab)) )
plt.show()
