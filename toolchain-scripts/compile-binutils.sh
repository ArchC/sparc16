#!/bin/bash

#rm -rf /p/toolchains/srcs/binutils-2.15
#tar xjf /p/toolchains/srcs/binutils-2.15.tar.bz2 -C \
#  /p/toolchains/srcs/
cd /p/archc-tools/sparc16 && rm -rf acbingenbuilddir
rm -f /usr/local/sparc16/* && acbingen.sh -i/usr/local/sparc16 sparc16.ac
