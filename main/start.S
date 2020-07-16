.global _start

_start:
@设置中断向量函数
	ldr pc, =_reset_handler				@复位中断函数
	ldr pc, =_undefined_handler			@未定义指令中断函数
	ldr pc, =_svc_handler				@SVC
	ldr pc, =_pre_abort_handler			@预取终止
	ldr pc, =_data_abort_handler		@数据终止
	ldr pc, =_not_used_handler			@未使用
	ldr pc, =_irq_handler				@
	ldr pc, =_frq_handler				@

@编写中断向量服务函数
_reset_handler:
	cpsid i	@禁止IRQ中断
	@ cpsie i	@使能IRQ中断
	@ cpsid f	@禁止FRQ中断
	@ cpsie f	@使能FRQ中断

	@关闭 I/D cache 和 MMU
	@ mrc/mcr {cond} p15, <opc1>, <Rt>, <CRn>, <CRm>, <opc2>
	mrc p15, 0, r0, c1, c0, 0		@ 读取 SCTLR 寄存器中的数据到 r0 寄存器中
	bic r0, r0, #(1 << 12)			@ 关闭I cache
	bic r0, r0, #(1 << 11)			@ 关闭分支预测
	bic r0, r0, #(1 << 2)			@ 关闭D cache
	bic r0, r0, #(1 << 1)			@ 关闭对齐
	bic r0, r0, #(1 << 0)			@ 关闭MMU
	mcr p15, 0, r0, c1, c0, 0		@ 将 r0 寄存器中的数据写入到 SCTLR 寄存器中

	@设置VBAR寄存器 中断向量偏移
	ldr r0, =0x87800000
	dsb	@数据同步隔离，仅当所有在它前面的存储器访问操作都执行完毕后，才执行它后面的指令
	isb @指令同步隔离，它会清洗流水线，以保证所有它前面的指令都执行完毕之后，才执行它后面的指令
	mcr p15, 0, r0, c12, c0, 0
	dsb
	isb

@清除bss数据段
.global _bss_top
_bss_top:
	.word __bss_top

.global _bss_tail
_bss_tail:
	.word __bss_tail

	ldr r0, _bss_top
	ldr r1, _bss_tail
	mov r2, #0

_bss_loop:
	stmia r0!, {r2}	@将r0寄存器所指的地址清零，后地址加1
	cmp r0, r1		@比较r0和r1的值
	ble _bss_loop	@如果r0!=r1

@设置SP指针
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	mrs r0, cpsr			@ 读取 cpsr 寄存器的值到 r0 寄存器
	bic r0, r0, #0x1f 		@ 清除低5位
	orr r0 ,r0, #0x12		@ 设置 IRQ 模式
	msr cpsr, r0			@ 将 r0 寄存器的值写入 cpsr 寄存器
	ldr sp, =0x80600000		@ 设置IRQ 模式的SP指针

	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	mrs r0, cpsr			@ 读取 cpsr 寄存器的值到 r0 寄存器
	bic r0, r0, #0x1f 		@ 清除低5位
	orr r0 ,r0, #0x1f		@ 设置 USER和SYS 模式
	msr cpsr, r0			@ 将 r0 寄存器的值写入 cpsr 寄存器
	ldr sp, =0x80400000		@ 设置 USER和SYS 模式的SP指针

	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	mrs r0, cpsr			@ 读取 cpsr 寄存器的值到 r0 寄存器
	bic r0, r0, #0x1f 		@ 清除低5位
	orr r0 ,r0, #0x13		@ 设置 SVC 模式
	msr cpsr, r0			@ 将 r0 寄存器的值写入 cpsr 寄存器
	ldr sp, =0x80200000		@ 设置SVC 模式的SP指针

	@cpsid i	@禁止IRQ中断
	cpsie i	@使能IRQ中断
	@cpsid f	@禁止FRQ中断
	@cpsie f	@使能FRQ中断

@跳转mian函数
_b_main:
	b main
@end of _reset_handler


_irq_handler:
	push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	@ mrc/mcr {cond} p15, <opc1>, <Rt>, <CRn>, <CRm>, <opc2>
	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0(CBAR)寄存器内的值到R1寄存器中
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0xC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存着当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */

	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irq_handler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc */

_frq_handler:
	ldr r0, =_frq_handler
	bx r0

_undefined_handler:
	ldr r0, =_undefined_handler
	bx r0

_svc_handler:
	ldr r0, =_svc_handler
	bx r0

_pre_abort_handler:
	ldr r0, =_pre_abort_handler
	bx r0
_data_abort_handler:
	ldr r0, =_data_abort_handler
	bx r0
_not_used_handler:
	ldr r0, =_not_used_handler
	bx r0
_end:



