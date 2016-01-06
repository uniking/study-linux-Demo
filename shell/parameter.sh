#!/bin/bash

# ./parameter 10 30
echo parameter number is $#
echo $0
echo $1
echo $2

myadd()
{
	if test $# -eq 2 #两个参数就执行加法操作，否则出错
	then 
		echo $[$1+$2]
		code=0
	else 
		echo parameter fail
		code=1
	fi

	return $code
}

echo myadd...
myadd 10 50 3

echo "please input your name:"
read name
echo "please input your password"
read -s password
echo "welcome $name"
echo "please input two numbers to add them:"
read numbers
myadd $numbers

echo "pleas input you likes books,I will wait for you 10 second"
read -t 10 book

