#!/bin/bash
# `command` 将命令的结果复值给以给变量
var=`ls`
varall=`ls -al`
echo my var is $var
echo myvar -al is $varall

# $(command)
echo myvar is $(ls)
