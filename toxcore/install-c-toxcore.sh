#!/bin/bash
#debian install c-toxcore

apt install make -y
apt install automake -y
apt install libtool -y
git clone https://github.com/jedisct1/libsodium.git
cd libsodium
./autogen.sh
./configure
make
make install
cd ../
git clone https://github.com/TokTok/c-toxcore.git
cd c-toxcore
./autogen.sh
./configure
make
make install
