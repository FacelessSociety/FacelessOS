#ifndef IOPL_H
#define IOPL_H

// 2022 Ian Moffett <ian@kesscoin.com>

// Sets IOPL to zero (i.e only ring 0 can use I/O instructions).
void zero_iopl(void);



#endif
