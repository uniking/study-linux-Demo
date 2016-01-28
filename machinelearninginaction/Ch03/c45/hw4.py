# decition tree C4.5
import time
from math import log
import operator

def loadDataSet():
    global labels
    # Load data set
    fr=open('hw4-task1-data.tsv')
    data=[]
    for line in fr:   data.append(line.strip().split('\t'))
    labels=['age','workclass','fnlwgt','education','education-num','marital-status','occupation','Farming-fishing','relationship','race','sex','capital-gain','capital-loss','hours-per-week','native-country']
    print "Number of records: %d" % len(data)   


    # Split training/test sets
    # You need to modify the following code for cross validation.
    K = 10
    training_set = [x for i, x in enumerate(data) if i % K != 9]
    test_set = [x for i, x in enumerate(data) if i % K == 9]
    return training_set,test_set

def  Label(dataSet):
    global currentLabel
    for line in dataSet: #the the number of unique elements and their occurance
        currentLabel = line[-1]
        if currentLabel not in labelList.keys(): 
            labelList[currentLabel] = 0
        labelList[currentLabel] += 1
    return labelList    

def calEntropy(dataSet):
    global labelList
    numEntries = len(dataSet)
    labelList = {}
    labelList=Label(dataSet)
    entropy = 0.0
    for key in labelList:
        prob = float(labelList[key])/numEntries
        entropy -= prob * log(prob,2) #log base 2
    return entropy

    
def splitDataSet(dataSet, axis, value):
    retDataSet = []
    for featVec in dataSet:
        if featVec[axis] == value:
            reducedFeatVec = featVec[:axis]     #chop out axis used for splitting
            reducedFeatVec.extend(featVec[axis+1:])
            retDataSet.append(reducedFeatVec)
    return retDataSet
    
def chooseBestFeatureToSplit(dataSet):
    numFeatures = len(dataSet[0]) - 1      #the last column is used for the labels
    baseEntropy = calEntropy(dataSet)
    bestInfoGainRatio = 0.0; bestFeature = -1
    for i in range(numFeatures):        #iterate over all the features
        featList = [example[i] for example in dataSet]#create a list of all the examples of this feature
        uniqueVals = set(featList)       #get a set of unique values
        newEntropy = 0.0
        for value in uniqueVals:
            subDataSet = splitDataSet(dataSet, i, value)
            prob = len(subDataSet)/float(len(dataSet))
            newEntropy += prob * calEntropy(subDataSet)     
        infoGainRatio = (baseEntropy - newEntropy)/baseEntropy     #calculate the info gain; ie reduction in entropy
        if (infoGainRatio > bestInfoGainRatio):       #compare this to the best gain so far
            bestInfoGainRatio = infoGainRatio         #if better than current best, set to best
            bestFeature = i
    return bestFeature,bestInfoGainRatio                      #returns an integer

def majorityCnt(classList):
    classCount={}
    for vote in classList:
        if vote not in classCount.keys(): classCount[vote] = 0
        classCount[vote] += 1
    sortedClassCount = sorted(classCount.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]

def createTree(dataSet,labels):
    classList = [example[-1] for example in dataSet]
    if classList.count(classList[0]) == len(classList): 
        return classList[0]#stop splitting when all of the classes are equal
    if len(dataSet[0]) == 1: #stop splitting when there are no more features in dataSet
        return majorityCnt(classList)
    bestFeat,bestInfoGainRatio = chooseBestFeatureToSplit(dataSet)
    if bestInfoGainRatio<1:   # make it as leaf node
    	return majorityCnt(classList)
    bestFeatLabel = labels[bestFeat]
    myTree = {bestFeatLabel:{}}
    del(labels[bestFeat])
    featValues = [example[bestFeat] for example in dataSet]
    uniqueVals = set(featValues)
    for value in uniqueVals:
        subLabels = labels[:]       #copy all of labels, so trees don't mess up existing labels
        myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet, bestFeat, value),subLabels)
    return myTree                            
    
def classify(inputTree,featLabels,testVec):
    firstStr = inputTree.keys()[0]
    secondDict = inputTree[firstStr]
    featIndex = featLabels.index(firstStr)
    key = testVec[featIndex]
    valueOfFeat = secondDict[key]
    if isinstance(valueOfFeat, dict): 
        classLabel = classify(valueOfFeat, featLabels, testVec)
    else: classLabel = valueOfFeat
    return classLabel

def Classify(tree,labels,test_set):
	result=[]
	for ins in test_set:
		tmp=classify(tree, labels, ins)
		result.append(tmp==ins[-1])
	accuracy = float(results.count(True))/float(len(results))
	print "accuracy: %.4f" % accuracy	

    


start_time=time.time()
training_set, test_set=loadDataSet()
tree=createTree(training_set, labels)
# Classify(test_set)
# end_time=time.time()
# print (end_time-start_time)
