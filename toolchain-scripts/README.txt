
STEPS
-----

1) install sparc-elf-4.4.1-1.0.32d.tar.bz2 from gaisler.com
2) make sure you have libmpfr.so.1 in /usr/lib
3) Get systemc 2.2, apply the patch in patches directory and run
compile-systemc.sh
4) Get archc from this branch:
svn co http://lampiao.lsc.ic.unicamp.br/svn/archc-prj/archc/branches/archc-revolutions-cardoso
5) Compile it with compile-arch.sh
6) Generate the binutils for sparc16 running: compile-binutils.sh
7) write the program using sparc16 assembly and make sure that
you change mode before running any instruction
8) use sparc16-elf-as to assemble
9) use sparc16-as-ld.sh to call sparc16-elf-ld with the right flags
and be ready for happiness.

p.s. before running any of those scripts make sure to hack the
paths they define.
