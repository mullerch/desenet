
SEARCH_DIR(.)
GROUP(-lgcc -lg -lstdc++)

_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x1000;

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
	.  = 0x0;
	.text :
	{
		KEEP(*(vectors))

		*(.text .text.* .gnu.linkonce.t.*)
		*(.rodata .rodata.* .gnu.linkonce.r.*)

		*(.eh_frame_hdr)
		*(.eh_frame)
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		*(.gcc_except_table)
		*(.eh_frame_hdr)
		*(.eh_frame)

		. = ALIGN(4);
		KEEP(*(.init))

		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;

		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;

		. = ALIGN(0x4);
		__CTOR_LIST__ = .;
		LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors)))
		KEEP (*crtend.o(.ctors))
		LONG(0)			/* end of list */
		__CTOR_END__ = .;

		. = ALIGN(4);
		KEEP(*(.fini))
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;

		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

		. = ALIGN(8);
		_etext = .;

	} >FLASH

	/* .ARM.exidx is sorted, so has to go in its own output section. */
	__exidx_start = .;

	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >FLASH
	__exidx_end = .;

	_flash_data = .;
	.data :
	{
		_data = .;
		*(vtable)
		*(.data*)
		_edata = .;
	} > SRAM AT > FLASH

	.bss :
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > SRAM			
	
	.stack :
	{
		. = ALIGN(16);
	   _stack_end = .;
		. += _STACK_SIZE;
		. = ALIGN(16);
		_stack = .;
	} > SRAM
	
	.heap :
	{
		. = ALIGN(16);
		_heap = .;
   		__cs3_heap_start = .;
		. += 0x100;
   		_end = .;
   		end = .;
   		__end = .;	
	} > SRAM
	
	note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
	.ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
	
	/* Remove the debugging information from the standard libraries */
	/DISCARD/ :
	{
		*(.note.GNU-stack)
		libc.a ( * )
		libgcc.a ( * )
		libm.a ( * )
		libg.a ( * )
		libstdc++.a ( * )
	}
	
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}

/* Provide end of the heap */
PROVIDE(_eheap = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM) - 8 ,8));
