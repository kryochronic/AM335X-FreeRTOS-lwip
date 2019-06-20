/*  BBB linker script */


MEMORY
{
    SRAM :     o = 0x402F0400,  l = 0x0000FC00  /* 64kB internal SRAM */
    L3OCMC0 :  o = 0x40300000,  l = 0x00010000  /* 64kB L3 OCMC SRAM */
    M3SHUMEM : o = 0x44D00000,  l = 0x00004000  /* 16kB M3 Shared Unified Code Space */
    M3SHDMEM : o = 0x44D80000,  l = 0x00002000  /* 8kB M3 Shared Data Memory */
    DDR0 :     o = 0x80000000,  l = 0x40000000  /* 1GB external DDR Bank 0 */
}

ENTRY(Entry)

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS {

    .startcode :
	{
		. = ALIGN(4);
        . = . + 256; /* Space for Vector Table here */
		*init.o (.text)       /* Required for CCS ways of linking */
		*init.S.obj (.text)   /* Required for non-CCS ways of linking */
	} >DDR0

	.text : 
	{
		. = ALIGN(4);
		*(.text*)
		*(.rodata*)
	} >DDR0

	.data :
	{
		. = ALIGN(4);
		*(.data*) 
	} >DDR0

	.services :
	{
		. = ALIGN(4);
		/******************************************************************************/
        _service_start = .;
        *(.init_fptr*)
        _services_end = .;
        /******************************************************************************/
	} >DDR0

	.bss :  
	{
		. = ALIGN(4); 
		_bss_start = .;
        __bss_start__ = _bss_start;
		*(.bss*)  
		*(COMMON)
		_bss_end = .; 
        __bss_end__ = _bss_end;
	} >DDR0

    .heap :
    {
        . = ALIGN(4);
        __end__ = .;
        end = __end__;
        __HeapBase = __end__;
        *(.heap*)
        . = . + 10M;
        __HeapLimit = .;
    } >DDR0

    .stack :
    {
        . = ALIGN(4);
        __StackLimit = . ;
        *(.stack*)
        . = . + 10M;
        __StackTop = .;
    } >DDR0
    _stack = __StackTop;


} 

/******************************************************************************/
__INIT_FPTR_START = _service_start;
__INIT_FPTR_END	  = _services_end;
