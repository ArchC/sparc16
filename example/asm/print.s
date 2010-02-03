	.file	"print.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LLC0:
	.asciz	"Hello Fuckers!! This is the sparc16 test number %d\n"
	.section	".text"
	.align 4
	.global main
main:
  sparcv8bx main_16
  nop32
main_16:
	savesp 16
	mov16	1, %o1
	esethi16	%hi(.LLC0), %o0
	eor16	%o0, %lo(.LLC0), %o0
	sparc16bx	printf
  nop
	mov16	0, %i0
	ret
	trestore # delay slot
	.size	main, .-main
