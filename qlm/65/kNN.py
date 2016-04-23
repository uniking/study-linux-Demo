'''
Created on Sep 16, 2010
kNN: k Nearest Neighbors


Input:      inX: vector to compare to existing dataset (1xN)
            dataSet: size m data set of known vectors (NxM)
            labels: data set labels (1xM vector)
            k: number of neighbors to use for comparison (should be an odd number)
            
Output:     the most popular class label

@author: pbharrin
'''
from numpy import *
import operator
from os import listdir

def file2matrix(filename, y):
    fr = open(filename)
    numberOfLines = len(fr.readlines())         #get the number of lines in the file
    returnMat = zeros((numberOfLines,y))        #prepare matrix to return  
    fr = open(filename)
    index = 0
    for line in fr.readlines():
	line = line.strip()
	line = line.strip('/n')
	'''listFromLine = line.split('\t')'''
	listFromLine = line.split(' ')
	'''print(listFromLine)'''
	returnMat[index,:] = listFromLine[0:y]
	index += 1
    return returnMat
    
