#!/bin/bash

# for var in list
# do
#    commands
# done

for test in Alabama Alaska Arizona Arkansas California Colorado
do
	echo The next state is $test
done


list="Alabama Alaska Arizona Arkansas California Colorado"
for test in $list
do
	echo List in $test
done

for test in `ls`
do
	echo  files in $test
done

#更改分割符
list="Alabama 1;Alaska 2;Arizona 2;Arkansas 3;California 4;Colorado 4"
IFS=$';'
for state in $list
do
	echo $state
done


for files in /home/*
do
	if test -d $files
	then  echo $files
	fi
done
