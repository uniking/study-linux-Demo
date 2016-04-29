from numpy import *
import operator
from os import listdir
from sklearn.ensemble import RandomForestClassifier
from sklearn.cross_validation import cross_val_score

def mat2data(data_set):
	labels = []
	data = []

	for line in data_set:
		c=0
		tmp_line=[]
		for attr in line:
			if c==0:
				c=c+1
				continue
			if c==1 or c==2 or c==3 or c==4 or c==5 or c==6 or c==7 or c==8 or c==9 or c==10:
				tmp_line.append(attr)
				c=c+1
			if c==11:
				labels.append(attr)
				c=c+1
		data.append(tmp_line)
	return labels, data

def mat2libsvm(filename):
	f_uf = open(filename)
	lebels = []
	savelist = []
	for line in f_uf.readlines():
		line = line.strip()
		line = line.strip('/n')
		listFromLine = line.split(' ')
		savelist.append(listFromLine)
	return mat2data(savelist)


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

def run_rf(data, lebals):
	clf=RandomForestClassifier(n_estimators=10)
	scores = cross_val_score(clf, data, lebals)
	print(scores.mean())

def run_pre_rf(data, lebals):
	clf=RandomForestClassifier(n_estimators=10)
	clf=clf.fit(data, lebals)
	lebals,data = mat2libsvm("check.txt")
	classT = clf.predict(data)
	print(classT)

def test_rf():
	lebals,data = mat2libsvm("train.txt")
	run_rf(data, lebals)


'''
strlist2file(mat2libsvm("train.txt", ',', 13),  "mod_svmdata.txt")
strlist2testfile(mat2libsvm("check.txt", ',', 13),  "mod_svmtestdata.txt")
'''

'''
print_class("ok.txt")
'''
test_rf()

