#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "usart.h"
#include "adc.h"


void TIM4_Inter_Init(u16 arr, u16 psc);

s8 pwm;
extern u8  TIM5CH2_CAPTURE_STA;	//���벶��״̬		    				
extern u32	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)


#endif
