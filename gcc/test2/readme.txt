[sxis@localhost test2]$ make
g++ -o test main.cpp \
/home/sxis/workspace/gcc/test2/a/libuse.a \
/home/sxis/workspace/gcc/test2/a1/libmyadd.a \
/home/sxis/workspace/gcc/test2/a2/libmysub.a
[sxis@localhost test2]$ make
g++ -o test main.cpp \
/home/sxis/workspace/gcc/test2/a1/libmyadd.a \
/home/sxis/workspace/gcc/test2/a2/libmysub.a \
/home/sxis/workspace/gcc/test2/a/libuse.a
/home/sxis/workspace/gcc/test2/a/libuse.a(use.o)：在函数‘use()’中：
use.cpp:(.text+0xf)：对‘myadd(int, int)’未定义的引用
use.cpp:(.text+0x1e)：对‘mysub(int, int)’未定义的引用
collect2: 错误：ld 返回 1
make: *** [all] 错误 1
[sxis@localhost test2]$ ls -R
.:
a  a1  a2  main.cpp  main.cpp~  makefile  makefile~

./a:
libuse.a  makefile  makefile~  use.cpp  use.cpp~  use.o

./a1:
libmyadd.a  makefile  makefile~  myadd.cpp  myadd.cpp~  myadd.o

./a2:
libmysub.a  makefile  makefile~  mysub.cpp  mysub.cpp~  mysub.o
[sxis@localhost test2]$ 

