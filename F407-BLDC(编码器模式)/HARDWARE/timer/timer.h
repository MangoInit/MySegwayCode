#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "usart.h"
#include "capture.h"

void Timer3_Init(u16 arr, u16 psc);

extern u16 encoder;								//¼ÆÊý

#endif
