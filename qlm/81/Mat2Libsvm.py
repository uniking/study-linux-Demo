from numpy import *
import operator
from os import listdir

letter = "?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

def char2int(onechar):
	cnt = 0
	cnt = letter.find(onechar, 0, len(letter))
	return cnt

def string2int(strname):
	count = 0;
	for letter in strname:
		count = count + char2int(letter);
	return count

def list2int(strlist):
	cnt = len(strlist)
	i = 0
	while i<cnt:
		strlist[i] = string2int(strlist[i])
		i=i+1
	return strlist

def int2stringlist(inlist):
	newlist = []
	for oneint in inlist:
		newlist.append(str(oneint))
	return newlist

def strlist2file(strlist, filename):
	fl = open(filename, 'w')
	for i in strlist:
		if(i[12] == "53"):
			fl.write("1")
			fl.write(" ")
		else:
			fl.write("-1")
			fl.write(" ")
		c=1
		for j in i:
			if c==13:
				break;
			fl.write(str(c))
			fl.write(":")
			fl.write(j)
			fl.write(' ')
			c=c+1
		fl.write("\n")
	fl.close()

def strlist2testfile(strlist, filename):
	fl = open(filename, 'w')
	for i in strlist:
		fl.write("1 ")
		c=1
		for j in i:
			fl.write(str(c))
			fl.write(":")
			fl.write(j)
			fl.write(' ')
			c=c+1
		fl.write("\n")
	fl.close()

def mat2libsvm(filename, csplit, column):
	f_uf = open(filename)
	savelist = []
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		listFromLine = line.split(',')
		print(listFromLine)
		intlistFromLine = list2int(listFromLine)
		print(intlistFromLine)
		strlistFromLine = int2stringlist(intlistFromLine)
		savelist.append(strlistFromLine)
		print(strlistFromLine)
	return savelist


def print_class(filename):
	f_uf = open(filename)
	savelist = ""
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		if line=="-1":
			savelist+="1"
		else:
			savelist+="0"
	print savelist


'''strlist2file(mat2libsvm("adult.txt", ',', 13),  "svmdata.txt")
strlist2testfile(mat2libsvm("adult_test.txt", ',', 13),  "svmtestdata.txt")
'''
print_class("ok.txt")
