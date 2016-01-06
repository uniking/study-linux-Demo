#!/bin/bash
#var=value  必须注意，var和=之间不能有任何符号
name=wang
fullname=$name
fullname+=zhi long
echo name
echo $name
echo "$fullname"
echo $(name)
