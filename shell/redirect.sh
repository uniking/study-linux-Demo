#!/bin/bash
# command > outputfile
# command < inputfile
ls > test.txt
echo ---show test.txt
cat test.txt
echo ---file info
wc < test.txt
