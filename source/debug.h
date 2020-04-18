#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <errno.h>

#define ftdines_printerr(...) printf("ftdines: %s():%d: ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);
#define ftdines_printf(...) printf("ftdines: "); printf(__VA_ARGS__);


#endif
