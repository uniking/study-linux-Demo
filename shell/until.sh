#!/bin/bash
var=10
until test $var -eq 0
do
	echo $var
	var=$[$var-1]
done
