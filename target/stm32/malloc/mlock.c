#include <malloc.h>
#include <sys/lock.h>
#include <critical/critical.h>


void __malloc_lock (ptr) struct _reent *ptr;
{
	enterCritical();
}

void __malloc_unlock (ptr) struct _reent *ptr;
{
	exitCritical();
}
