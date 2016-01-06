#!/bin/bash

# while test command
# do
#	other commands
# done

var=10
while test $var -gt 1
do
	echo $var
	var=$[$var-1]
done

