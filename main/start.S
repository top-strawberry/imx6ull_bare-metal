.global _start

.global _bss_top
_bss_top:
	.word __bss_top

.global _bss_tail
_bss_tail:
	.word __bss_tail

_start:
	mrs r0, cpsr
	bic r0, r0, #0x1f 
	orr r0 ,r0, #0x13
	msr cpsr, r0

	ldr r0, _bss_top
	ldr r1, _bss_tail
	mov r2, #0x0

_clear_bss:
	stmia r0!, {r0}	@将r0寄存器所指的地址清零，后地址加1
	cmp r0, r1		@比较r0和r1的值
	ble _clear_bss	@如果r0!=r1

_b_main:
	ldr sp, =0x80200000
	b main
