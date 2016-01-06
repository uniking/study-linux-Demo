#!/bin/bash
# $[number]
num1=100
num2=200
mul=$[$num1*$num2]
echo $[1+5]
echo [1+5]
echo $[$num1+$num2]
echo mul=$mul

#浮点数
#sca是bc命令的内建变量，定义有效符号位数
echo `echo "scal=4; 13.3333*8" | bc`
var1=`echo "scal=4; 12.6*3" | bc`
echo "12.6*3=$var1"
