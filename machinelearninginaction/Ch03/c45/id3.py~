"""
This module contains the functions for calculating the information infogain of a
data set as defined by the ID3.
"""
from dmP.classification.DecisionTree.Splitter import Splitter


class ID3(Splitter):    
    """
    ID3 algorithm
    """
    
    def splitmetric(self, dataset, attr, target_attr):
        """
        Information Gain for ID3 algorithm
        It calculates the information gain (reduction in entropy) that would
        result by splitting the data on the chosen attribute.
        """
        freq = {}
        subset_entropy = 0.0
        samplenumbers = len(dataset)
        # Calculate the frequency of each of the values in the split attribute
        for record in dataset:
            if (record[attr] in freq):
                freq[record[attr]] += 1.0
            else:
                freq[record[attr]] = 1.0
    
        # Calculate the sum of the entropy for each subset of records weighted
        # by their probability of occurring in the training set.
        for val in list(freq.keys()):
            val_prob = freq[val] / samplenumbers
            data_subset = [record for record in dataset if record[attr] == val]
            subset_entropy += val_prob * Splitter.entropy(self, data_subset, target_attr)
    
        # Subtract the entropy of the chosen attribute from the entropy of the
        # whole data set with respect to the target attribute.
        return (Splitter.entropy(self, dataset, target_attr) - subset_entropy)
    
    