#!/bin/bash
# if command1 then command2 如果command1成立则执行command2
# 测试的为command1的退出码是否为0
# if 和 then不能在统一行内，无法区分then是否为命令的一个参数
if ls 
then echo it worked
fi

xxxx=aaaa
yyyy=ls
if $xxxx
then echo xxxx is command
else echo xxxx is not command
fi

if $xxxx
then echo xxxx is command
elif $yyyy
then echo yyyy is command
fi

#逻辑判断  数值比较 字符串比较 文件比较
# test condition 或 [condition] 测试中我的bash不支持[condition]
name1=wang
name2=wang
if test  $name1 = $name2
then echo "name1 == name2"
elif test $name1 > $name2
then echo "name1 > name2"
fi

num1=100
num2=200
if test $num1 -eq $num2
then echo "num1==num2"
elif test $num1 -ge $num2
then echo "num1>=num2"
elif test $num1 -gt $num2
then echo "num1>num2"
elif test $num1 -lt $num2
then echo "num1 < num2"
fi

file1=`pwd`
file2=$file1/xxx
if test -d $file1
then echo $file1 is directory
fi

# !
if ! test -d $file1
then echo $file1 is not directory
fi

if test -e $file2
then echo $file2 is exist
else echo $file2 is not exist
fi

if test -d $file1 && test -e $file2
then echo $file1 is directory and $file2 is exist
else echo fail
fi

#[ condition ] 取代 test condition
if [ $? -eq 0 ]
then echo exitcode == 0
fi
