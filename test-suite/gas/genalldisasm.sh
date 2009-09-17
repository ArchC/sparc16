#!/bin/bash

for i in `\ls *.s`; do
  sparc16-elf-as $i -o `basename $i .s`.o
  sparc16-elf-objdump -d `basename $i .s`.o | grep -E \
    "^ *[0-9a-f]+:" > `basename $i .s`.d
done
rm *.o
