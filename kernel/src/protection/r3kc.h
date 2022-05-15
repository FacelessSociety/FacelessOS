#ifndef R3KC_H
#define R3KC_H

#include <stdint.h>


/*
 *  2022 Ian Moffett <ian@kesscoin.com>
 *
 *
 *  R3KC (Ring 3 Kernel Callback).
 *  This file gives an interface
 *  for a driver to call a routine
 *  in ring3 without risking getting
 *  stuck in ring 0.
 */


/*
 *  If you are not going to supply an optional argument
 *  than you would probably want to set oparg0 to 0xDEAD
 *  which is R3KC standard.
 */

void r3kc_exec_callback(void* routine, uint32_t oparg0);


#endif
