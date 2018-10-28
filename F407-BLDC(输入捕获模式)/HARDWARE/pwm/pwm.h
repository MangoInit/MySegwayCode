#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM8_PWM_Init(u32 arr,u32 psc);
void PWM_Out(u16 PWM);
void Motor_Turn_Init(void);

#define Turn1 PAout(2)	//电机正反转
#define Turn2 PAout(3)	//1：正     0：反

#endif
