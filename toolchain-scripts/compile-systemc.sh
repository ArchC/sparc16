#!/bin/sh

echo -n "applying patch..."
patch -p0 < srcs/systemc-gcc4.3.patch
echo "OK"

sudo mkdir -p /usr/local/systemc-2.2
mkdir -p sandbox/systemc-2.2/objdir

cd sandbox/systemc-2.2/objdir

../../../systemc-2.2.0/configure --prefix=/usr/local/systemc-2.2

make && sudo make install
