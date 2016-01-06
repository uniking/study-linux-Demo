#!/bin/bash

string1="wang zhi long" 

# ${#varname} to get string length
echo "wang zhi long length=${#string1}"


# ${varname:start:length} to get substring
echo "start string1[3] print=${string1:4}"
echo "start string1[3] and end string[3+7] print=${string1:4:7}"



string2=${string1}" who are you?"
echo $string2

string3="c:/test/"${string2}"/1.txt"

# ${varname#substring正则表达式} 从字符串开头开始匹配substring，删除匹配的表达式
# ${varname%substring正则表达式} 从字符串结尾开始匹配substring，删除匹配的表达式
echo $string3
echo ${string3#*/}
echo ${string3%/*}
