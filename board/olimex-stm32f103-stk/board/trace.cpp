
#include <trace/trace.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


char strTrace[256];


// static
void Trace::out( const char * const format , ... )
{
	va_list args;

	va_start(args, format);
	vsiprintf(strTrace, format, args);
	va_end(args);

	const int len = strlen(strTrace);

	if (strTrace[len - 1] != '\n')
	{
		// Add "\r\n"
//		strTrace[len] 	  = '\r';
//		strTrace[len + 1] = '\n';
//		strTrace[len + 2] = '\0';

		// Add "\r" (linux host)
		strTrace[len] 	  = '\r';
		strTrace[len + 1] = '\0';
	}

	puts(strTrace);
}

void Trace::outToString( char * destination , const char * const format , ... )
{
	va_list arguments;

	va_start( arguments , format );

	// Print to buffer.
	vsiprintf( destination , format , arguments );

	va_end( arguments );
}


void Trace::outToString( char * destination , size_t size , const char * const format , ... )
{
	va_list arguments;

	va_start( arguments , format );

	// Print to buffer.
	vsniprintf( destination , size , format , arguments );

	va_end( arguments );
}

