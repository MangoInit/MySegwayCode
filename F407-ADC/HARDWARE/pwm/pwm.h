#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM9_PWM_Init(u32 arr,u32 psc);
void PWM_Out(u16 PWM);
#endif
