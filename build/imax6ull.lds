SECTIONS
{
	. = 0x87800000;
	.text :
	{
		../obj/start.o
		*(.text)
	}
	.rodata ALIGN(4) : {*(.rodata*)}
	.data ALIGN(4) : {*(.data)}
	. = ALIGN(4);
	__bss_top = . ;
	.bss ALIGN(4) : {*(.bss) *(COMMON)}
	. = ALIGN(4);
	__bss_tail = . ;

}
