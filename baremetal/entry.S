
# entry of the bare metal program
.globl _start
_start:
	b reset
	b .
	b .
	b .
	b .
	b .
	b .
	b .

.global reset
reset:
	ldr sp, =0x93ff20
	@ldr sp, =STACK_ADDR
	bic sp, sp, #7 /* 8-byte alignment for ABI compliance */
	bl bootmain
	b .
