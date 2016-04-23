from numpy import *
import operator
from os import listdir

f_uf = open("uv.txt")
s=set()
for line in f_uf.readlines():
	line = line.strip()
	line = line.strip('/n')
	listFromLine = line.split(' ')
	print(listFromLine)
	s.add(listFromLine[1])
print(s)
print(len(s))
