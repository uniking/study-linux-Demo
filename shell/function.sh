#!/bin/bash

function myprint
{
	echo "Hello world"
}

myprint2()
{
	echo "Hello world2"
	return 1
}


#main()
myprint
echo exit code=$?
myprint2
echo exit code=$?
