#!/bin/bash

readelf -r /usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/../../../../sparc-elf/lib/locore_mvt.o \
/usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/../../../../sparc-elf/lib/crt0.o \
/usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/crti.o \
/usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/crtbegin.o \
/usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/crtend.o \
/usr/sparc-elf-4.4.1/bin/../lib/gcc/sparc-elf/4.4.1/crtn.o \
  | awk '/00[0-9a-f]+/ {print $3}' | sort | uniq 
