import matplotlib.pyplot as plt
from numpy import *

def standplot(xArr, yArr, ws):
    xMat=mat(xArr)
    yMat=mat(yArr)
    yHat=xMat*ws
    fig=plt.figure()
    ax=fig.add_subplot(111)
    ax.scatter(xMat[:,1].flatten().A[0], yMat.T[:,0].flatten().A[0])
    xCopy=xMat.copy()
    xCopy.sort(0)
    yHat=xCopy*ws
    ax.plot(xCopy[:,1], yHat)
    plt.show()

def lwlrplot(yHat, xArr, yArr):
    xMat=mat(xArr)
    srtInd = xMat[:,1].argsort(0)
    xSort = xMat[srtInd][:,0,:]
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(xSort[:,1], yHat[srtInd])
    ax.scatter(xMat[:,1].flatten().A[0], mat(yArr).T.flatten().A[0], s=2, c='red')
    plt.show()

def printline(line):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(line)
    plt.show()
