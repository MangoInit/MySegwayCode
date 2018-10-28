#ifndef __CAPTURE_H
#define __CAPTURE_H
#include "sys.h"

extern u32 cap_val;

void Capture_Init(void);
u32 Read_Capture(u8 TIMX);

#endif
