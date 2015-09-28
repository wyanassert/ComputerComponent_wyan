#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
    char expr[32];
    int oldValue;
    int newValue;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */


} WP;

#endif
