#ifndef	__CAPTURE_H
#define __CAPTURE_H
#include "sys.h"
#include "usart.h"
//u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
//u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//u8  TIM2CH3_CAPTURE_STA=0;	//���벶��״̬		    				
//u32	TIM2CH3_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM2_CH3_Cap_Init(u32 arr,u16 psc);
u32 Read_Speed(u8 TIMX);


#endif
