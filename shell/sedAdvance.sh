#!/bin/bash
echo ----try s
sed 's/dog but cat/dog/' sedData

#N 合并两行  . 统配 空格和换行
echo ---N try s
sed 'N ; s/dog .but cat/dog/' sedData

#N d  多行删除
echo ---N d
sed 'N; /dog .but/d' sedData
