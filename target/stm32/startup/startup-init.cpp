/*
 * startup-init.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: sth
 */


// Variables defined in linker script
extern unsigned long _flash_data;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;


// Following function pointers are defined in the linker script
extern "C" void (*__init_array_start []) (void);
extern "C" void (*__init_array_end []) (void);
extern "C" void (*__fini_array_start []) (void);
extern "C" void (*__fini_array_end []) (void);
extern "C" void (*__CTOR_LIST__ []) (void);


/**
 * Calls global/static constructors
 */
void init_array()
{
  unsigned int size;

	size =  __init_array_end - __init_array_start;
	for (unsigned int i = 0; i < size; i++)
	{
		__init_array_start [i] ();
	}
}

/**
 * Calls global/static destructors
 */
void fini_array()
{
	for (int i = __fini_array_end - __fini_array_start - 1; i >= 0; i--)
	{
		__fini_array_start [i] ();
	}
}

/**
 * Calls constructors when using arm-elf toolchain
 */
void __do_global_ctors()
{
	unsigned long nptrs = (unsigned long) __CTOR_LIST__[0];

	if (nptrs == (unsigned long)-1)
	{
		for (nptrs = 0; __CTOR_LIST__[nptrs + 1] != 0; nptrs++);
	}

	for (unsigned i = nptrs; i >= 1; i--)
	{
		__CTOR_LIST__[i]();
	}
}

