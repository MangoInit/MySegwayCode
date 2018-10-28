#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "usart.h"
#include "adc.h"


void TIM4_Inter_Init(u16 arr, u16 psc);

s8 pwm;
extern u8  TIM5CH2_CAPTURE_STA;	//输入捕获状态		    				
extern u32	TIM5CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)


#endif
