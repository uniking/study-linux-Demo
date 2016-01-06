#!/bin/bash
# sed options script file        script用单引号

# /搜索串
# /搜索串/替换串
# 命令\字符串

#s命令 s/des string/src string/
echo "This is a test" | sed 's/test/good test/'

# -e 多条指令
echo "This is a green dog" | sed -e 's/green/black/  ; s/dog/cat/'

#-f 使用脚本文件
echo "This is a green dog" | sed -f sedScript

#
echo ....................
echo test filedata
sed -e 's/green/black/ ; s/dog/cat/' sedData
echo ....................

#只替换了每行的第一个匹配
sed 's/green/black/' sedData

echo ................
echo num of line to s
sed 's/green/black/2' sedData

echo .......... p .. #打印替换后的那一行
sed 's/green/black/p' sedData

echo .......... -n   p .. # -n禁止sed输出结果
sed -n 's/green/back/p' sedData

echo .........w  #将修改过的行写入文件
sed -n 's/green/back/w sedw.txt' sedData
cat sedw.txt

echo .......d  delete line
sed '2d' sedData
echo .....d delete green line
sed '/green/d' sedData

echo ..... insert one line befor address #行号 命令\ 新行内容
sed '1i\0 hello' sedData

echo .... appand one line after address
sed '1a\0 hello' sedData

echo .... change one line
sed '1c\hello' sedData
