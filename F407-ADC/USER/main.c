/*************************************
功能：平衡车转向轴ADC数据抓取
备注：每次启动前转向轴的位置为初始位置
调试：手机蓝牙串口调试，波特率9600
时间：2017年10月20日
作者：Mango
**************************************/
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"
#include "adc.h"
int main(void)
{ 
	u16 Init_ADC_Value;															//有符号原始ADC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组2
	delay_init(168);  															//初始化延时函数
	uart_init(9600);																//初始化串口波特率为115200
	LED_Init();
	Adc_Init();
//	TIM9_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
//	PWM_Out(1);					//0-499
	Init_ADC_Value = (uc16)Get_Adc_Average(ADC_Channel_1,20);				//初始轴位置
	while(1)
	{
//		LED_Blink();
		u16 adcx;
		s16 adcx_treated;
		float temp;
		u8 t;
		adcx = Get_Adc_Average(ADC_Channel_1,20);					//获取通道1的转换值，20次取平均
		adcx_treated = (s16)adcx - (u16)Init_ADC_Value;				//轴转动采集数据(顺时针为正，逆时针为负)
		temp = (float)adcx*(3.3/4096);								//显示ADC采样后的原始值（2^12位)
//		adcx = temp;												//赋值整数部分，adcx为u16位整形
//		temp = temp - adcx;											//小数部分
//		temp = temp * 1000;											//保留三位小数
		delay_ms(10);
		LED0=!LED0;
		LED1=!LED1;
		printf("%3d:ADC = %4d		turn = %5d			Init_ADC = %d\r\n",t, adcx, adcx_treated, Init_ADC_Value);
		t++;
	}
}

