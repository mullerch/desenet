/*
 * startup-init.h
 *
 *  Created on: Feb 14, 2011
 *      Author: sth
 */

#ifndef STARTUP_INIT_H_
#define STARTUP_INIT_H_


void init_array();
void fini_array();

void __do_global_ctors();


#endif /* STARTUP_INIT_H_ */
