import trees
import hw4
import treePlotter

def testID3(filename):
    DataList,classLabelVector = trees.file2strlist(filename)
    mytree=trees.createTree(DataList,classLabelVector)
    treePlotter.createPlot(mytree)

def testC45(filename):
    DataList,classLabelVector = trees.file2strlist(filename)
    mytree=hw4.createTree(DataList,classLabelVector)
    treePlotter.createPlot(mytree)
