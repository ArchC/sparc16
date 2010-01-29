#!/bin/sh

ARCH_SRC="archc-revolutions-cardoso"
sudo rm -rf /usr/local/archc && sudo mkdir -p /usr/local/archc

gmake clean 2>/dev/null
gmake distclean 2>/dev/null
cd $ARCH_SRC
./configure --prefix=/usr/local/archc \
  --with-systemc=/usr/local/systemc-2.2 \
  --with-binutils=/p/toolchains/srcs/binutils-2.15

gmake && sudo gmake install
