#!/bin/bash

cat $1
sparc16-elf-as $1 -o `basename $1 .s`.o
sparc16-elf-objdump -rd `basename $1 .s`.o
