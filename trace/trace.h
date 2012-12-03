#pragma once
#include <cstring>

/**
 * @defgroup trace_group Trace
 * @brief The Trace package offers the possibility to output debug messages either to the debugger or only other channel.
 * @{
 *
 * @section trace_cd Class Diagram
 * @brief The trace package enables to output data to the debugger, a serial port or whatever...
 * @image html Trace.png
 * See Trace for details.
 */


/**
 * @brief The trace class can be used to send messages to the debugger, serial port or USB host PC, depending the
 * target and the possibilities it offers where the application is running on.
 */
class Trace
{
public:
	/*!
	 * @brief Trace information. The format and parameters are similar to the C's printf() function or Qt's qDebug() function.
	 *
	 * @param format The format of the output.
	 */
	static void out( const char * const format , ... );

	/*!
	 * @brief Trace information into the given string buffer. The format and parameters are similar to the C's
	 * 		  sprintf() function.
	 *
	 * @param destination 	The string buffer to trace into.
	 * @param format 		The format of the output.
	 */
	static void outToString( char * destination , const char * const format , ... );

	/*!
	 * @brief Trace information into the given string buffer. The format and parameters are similar to the C's
	 * 		  sprintf() function.
	 *
	 * @param destination 	The string buffer to trace into.
	 * @param size 			The maximal amount of characters to print into the string.
	 * @param format 		The format of the output.
	 */
	static void outToString( char * destination , size_t size , const char * const format , ... );
};

/** @} */
