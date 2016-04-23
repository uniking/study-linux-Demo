import sys
import kNN
from numpy import *
import matplotlib
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(111)
mat = kNN.file2matrix('123.txt', 2)

'''ax.scatter(mat[:,0], mat[:,1], 2, color ='blue') '''
ax.scatter(mat[:,1], mat[:,0], 2, color ='blue')
plt.show()
