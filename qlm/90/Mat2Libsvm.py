from numpy import *
import operator
from os import listdir

letter = "?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

def sigmoid(inX):
    return 1.0/(1+inX)

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
		if(i[11] == "1"):
			fl.write("1")
			fl.write(" ")
		else:
			fl.write("-1")
			fl.write(" ")

		c=0
		index=1
		for j in i:
			if c==1 or c==2 or c==3 or c==4 or c==5 or c==6 or c==7 or c==8 or c==9 or c==10:
				fl.write(str(index))
				fl.write(":")
				fl.write(j)
				fl.write(' ')
				index=index+1
			c=c+1
		fl.write("\n")
	fl.close()

def strlist2testfile(strlist, filename):
	fl = open(filename, 'w')
	for i in strlist:
		fl.write("1 ")
		c=0
		index=1
		for j in i:
			if c==1 or c==2 or c==3 or c==4 or c==5 or c==6 or c==7 or c==8 or c==9 or c==10:
				fl.write(str(index))
				fl.write(":")
				fl.write(j)
				fl.write(' ')
				index=index+1
			c=c+1
		fl.write("\n")
	fl.close()

def mod_in_list(line):
	new_line = []
	c=0
	for i in line:
		if c==11:
			new_line.append(i)
			c=c+1
			continue
		new_line.append(str(sigmoid(float(i))))
		c=c+1
	return new_line

def mat2libsvm_mod(filename, csplit, column):
	f_uf = open(filename)
	savelist = []
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		listFromLine = line.split(' ')
		mod_list = mod_in_list(listFromLine)
		savelist.append(mod_list)
	return savelist

def data_ok(a_l):
	if a_l[1]< 3000-1732 | a_l[1]>3000+1732:
		return false

	return true

def mat2libsvm(filename, csplit, column):
	f_uf = open(filename)
	savelist = []
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		listFromLine = line.split(' ')
		savelist.append(listFromLine)
	return savelist


def print_class(filename):
	f_uf = open(filename)
	savelist = ""
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		if line=="1":
			savelist+="1"
		else:
			savelist+="0"
	print savelist

'''
strlist2file(mat2libsvm("train.txt", ',', 13),  "mod_svmdata.txt")
strlist2testfile(mat2libsvm("check.txt", ',', 13),  "mod_svmtestdata.txt")
'''


print_class("ok.txt")
