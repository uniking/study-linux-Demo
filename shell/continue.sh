#!/bin/bash
var=10
while test $var -gt 0
do
	var=$[$var-1]
	if test $var -eq 5
	then continue #break
	fi
	echo $var
done
