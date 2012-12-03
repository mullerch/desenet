#include <inc/config.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#if (USE_USB_TRACE != 0)
	#include <vcom/usb_vcom.h>
	#include <board/usb-vcom/vcom.h>
#endif

#if (USE_USART2_TRACE != 0)
	#include <stm32f10x_usart.h>
#endif

// Function declaration.
void _exit(int i);
int _open(const char *name, int flags, int mode);
int _read(int file, char *ptr, int len);
int _write(int file, char *buffer, unsigned int count);
int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _link(char *old, char *new);
int _unlink(char *name);
int _stat(char *file, struct stat *st);
int _close(int file);
int _execve(char *name, char **argv, char **env);
int _fork();
int _getpid();
int _isatty(int file);
int _kill(int pid, int sig);
caddr_t _sbrk(int incr);
caddr_t _sbrk_r (struct _reent *, ptrdiff_t incr);
int times(struct tm *buf);
int _wait(int *status);

#undef errno
extern int errno;
char *__env[1] = {0};
char **__environ = __env;
extern unsigned int _heap;
extern unsigned int _eheap;
static caddr_t heap = NULL;


// Add dso_handle constant in case it does not exist.
// Used with shared libraries. Therefore not needed
// in an embedded system with no RTOS.
#ifndef __dso_handle
	int __dso_handle;
#endif

// Function definition.
void _exit(int i)
{
	iprintf("Program exit with code %d", i);
	while (1);
}

int _write(int file, char *ptr, unsigned int len)
{
/*	register int i;
	for (i=0; i<count; ++i) {
		putChar(*buffer++);
	}
	return count;
*/
	if (len < 0) return -1;
#if (USE_USB_TRACE != 0)
//	Virtual_Com_Port_Transmit((u8*) ptr, len);
	vcomWrite(ptr, len);
	return len;
#else
	#if (USE_USART2_TRACE != 0)
	{
		unsigned int i;

		for (i = 0; i < len; i++)
		{
			// Send next character
			USART_SendData(USART2, ptr[i]);

			// Wait until previous character got transfered
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		}
		return len;
	}
	#else
		return 0;
	#endif
#endif
}

int _read(int file, char *ptr, int len)
{
//	return 0;

	if (len < 0) return -1;
#if (USE_USB_TRACE != 0)
	return Virtual_Com_Port_Receive((u8*) ptr, len);
#else
	return 0;
#endif
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

caddr_t _sbrk(int incr)
{
	caddr_t prevHeap;
	caddr_t nextHeap;

	if (heap == NULL)
	{ // first allocation
		heap = (caddr_t) & _heap;
	}

	prevHeap = heap;

	// Always return data aligned on a 8 byte boundary
	nextHeap = (caddr_t) (((unsigned int) (heap + incr) + 7) & ~7);

	// Check enough space and there is no collision with stack coming the other way
	// if stack is above start of heap
	if (nextHeap >= (caddr_t) & _eheap)
	{
		errno = ENOMEM;
		return NULL; // error - no more memory
	}
	else
	{
		heap = nextHeap;
		return (caddr_t) prevHeap;
	}
}

caddr_t _sbrk_r (struct _reent * r, ptrdiff_t incr)
{
	return _sbrk(incr);
}

int _open(const char *name, int flags, int mode)
{
	return -1;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

int _fork()
{
	errno = EAGAIN;
	return -1;
}

int _getpid()
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return (-1);
}

int times(struct tm *buf)
{
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}
