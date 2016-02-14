import sys
from numpy import *
import matplotlib
import matplotlib.pyplot as plt
import svmMLiA

def printdata(Mat):
    fig = plt.figure()
    ax = fig.add_subplot(111)

    for dot in Mat:
        ax.scatter(dot[0], dot[1])

    plt.show()

def printsvm(Mat, label, alphas, b):
    fig = plt.figure()
    ax = fig.add_subplot(111)

    print 'to print scatter'
    i = 0
    for dot in Mat:
        if alphas[i] > 0.0:
            ax.scatter(dot[0], dot[1], c='r', marker='o')
        else:
            ax.scatter(dot[0], dot[1])
        i+=1

    print 'to print line'
    ws = svmMLiA.calcWs(alphas, Mat, label)
    x = arange(0:3:2)
    print 'x=',x
    y = ws*x+b
    print y
    ax.plot(x, y)

    plt.show()

def printsvm2(alphas, b):
    fig = plt.figure()
    ax = fig.add_subplot(111)

    print 'to print line'
    x = arange(100)
    print 'x=',x
    y = alphas.T()*x+b
    print 'y=', y
    ax.plot(x, y)

    plt.show()
