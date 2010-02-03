#!/bin/bash

[ ! -f print ] && echo "print executable not found" && exit

if [ "3" == "`sparc-elf-objdump -m sparc -d print | grep -i main | wc -l`" ];
then 
  echo "Linkage OK"
else
  echo "Linkage FAILED"
fi;
