#!/bin/bash

linker_script="/p/archc-tools/sparc16/toolchain-scripts/sparcleon.x"
#linker_script="/home/ecco/mestrado/pack/sparc16/toolchain-scripts/sparcleon.x"
gaisler_toolchain_dir="/opt/sparc-elf-4.4.1/bin"

sparc16-elf-ld --script=$linker_script -o $1 \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/../../../../sparc-elf/lib/locore_mvt.o \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/../../../../sparc-elf/lib/crt0.o \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/crti.o \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/crtbegin.o \
-L$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1 \
-L$gaisler_toolchain_dir/../lib/gcc \
-L$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/../../../../sparc-elf/lib \
$1.o -lc -lgcc -lleonbare -lc \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/crtend.o \
$gaisler_toolchain_dir/../lib/gcc/sparc-elf/4.4.1/crtn.o

