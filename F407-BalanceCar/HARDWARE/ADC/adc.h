#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
extern u16 Init_Steer_Value;			//初始转向轴的ADC数据
extern s16 Steer_Value;				//转向轴采集数据(正负之分)

void Turn_Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
void Steer_Init(void);
int Get_Steer(void);


#endif
