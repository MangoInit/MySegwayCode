/*******************************************************
在stm32F4开发板上,通过KEY2和KEY3控制pwm占空比的增加与较少,
从而控制PF8上PWM的pwm输出(极性高)
*******************************************************/
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"
#include "key.h"
#include "lcd.h"
#include "timer.h"
#include "capture.h"

int main(void)
{ 
	u8 T = 0;											//按键参数
	s8 pwm = 20;											//pwm值
//	s16 num=0;
//	u8 string1[5];										//显示字符串
//	u8 string2[5];										//显示字符串
//	u8 string3[5];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	delay_init(168);  									//初始化延时函数
	uart_init(115200);									//初始化串口波特率为115200
	TIM8_PWM_Init(500-1,84-1);							//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
	Motor_Turn_Init();									//电机转向控制初始化
	Key_GPIO_Config();									//按键初始化
//	Timer5_Encoder_Init();								//编码器初始化
	Capture_Init();
//	Timer3_Init(5000-1,8400-1);							//100ms中断一次
	
	
//	//LCD屏幕显示设置
//	LCD_Init();							//LCD屏幕初始化
//	POINT_COLOR=GRED;					//画笔颜色
//	LCD_Clear(BLACK);					//背景颜色为黑色
//	LCD_ShowString(60,10,200,24,24,"BalanceCar");
//	LCD_ShowString(50,40,200,16,16,"Speed:");
//	LCD_ShowString(50,60,200,16,16,"PWM  :");
//	LCD_ShowString(50,80,200,16,16,"text :");
	
	while(1)
	{ 
		T = Key_Scan(0);								//扫描按键
		if(T == 2)	    pwm = pwm + 4;
		else if(T == 3) pwm = pwm - 4;
		if(pwm > 70)	pwm = 0;
		if(pwm <= 0)	pwm = 0;
		if(pwm%8==0)
		{
			Turn1 = 1;									//电机正转
		}
		else
			Turn1 = 0;
		PWM_Out(pwm);									//输出pwm

//		sprintf((char*)string1,"%5d",num);
//		sprintf((char*)string2,"%5d",pwm);
//		sprintf((char*)string3,"%5d",num);
//		LCD_ShowString(140,40,200,16,16,string1);
//		LCD_ShowString(140,60,200,16,16,string2);
//		LCD_ShowString(140,80,200,16,16,string3);
	}
}

