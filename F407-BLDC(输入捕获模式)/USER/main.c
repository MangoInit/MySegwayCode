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
#include "hall.h"
#include "lcd.h"
#include "timer.h"
#include "math.h"
#include "adc.h"

extern s8 pwm;

int main(void)
{ 
	u8 T = 0;											//按键参数
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	delay_init(168);  									//初始化延时函数
	uart_init(115200);									//初始化串口波特率为115200
	TIM8_PWM_Init(500-1,84-1);							//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
	TIM5_CH2_Cap_Init(0xFFFFFFFF,84-1); 				//以1Mhz的频率计数 
	Motor_Turn_Init();									//电机转向控制初始化
	Key_GPIO_Config();									//按键初始化
	Adc_Init();											//内部温度传感器ADC初始化
	TIM4_Inter_Init(500-1,8400-1);						//50ms中断一次
	
	while(1)
	{ 
		T = Key_Scan(0);								//扫描按键
		
		if(T == 2)	    pwm = abs(pwm) + 2;
		else if(T == 3) pwm = pwm - 2;
		
		if(pwm > 100)	pwm = 0;
		
		if(pwm%8==0)
		{
			Turn1 = 1;									//电机正转
			pwm = -pwm;
		}
		else
		{
			Turn1 = 0;
		}
		PWM_Out(abs(pwm));									//输出pwm
	}	
}





//int main(void)
//{ 
//	u8 T = 0;											//按键参数
//	u16 t = 0;											//打印次数
//	s8 pwm = 12;											//pwm值
//	u32 temp = 0;									//text
//	u32 speed;
//	u16 i;												//运行状态
//	s16 num=0;
//	u8 string1[5];										//显示字符串
//	u8 string2[5];										//显示字符串
//	u8 string3[5];
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
//	delay_init(168);  									//初始化延时函数
//	uart_init(115200);									//初始化串口波特率为115200
//	TIM8_PWM_Init(500-1,84-1);							//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
//	TIM5_CH2_Cap_Init(0xFFFFFFFF,84-1); 				//以1Mhz的频率计数 
//	Motor_Turn_Init();									//电机转向控制初始化
//	Key_GPIO_Config();									//按键初始化
//	
//	LCD_Init();							//LCD屏幕初始化
//	POINT_COLOR=GRED;					//画笔颜色
//	LCD_Clear(BLACK);					//背景颜色为黑色
//	LCD_ShowString(60,10,200,24,24,"BalanceCar");
//	LCD_ShowString(50,40,200,16,16,"Speed:");
//	LCD_ShowString(50,60,200,16,16,"PWM  :");
//	LCD_ShowString(50,80,200,16,16,"text :");
//	
//	while(1)
//	{ 
//		T = Key_Scan(0);								//扫描按键
//		if(T == 2)	    pwm = pwm + 4;
//		else if(T == 3) pwm = pwm - 4;
//		if(pwm > 70)	pwm = 0;
//		if(pwm <= 0)	pwm = 0;
//		if(pwm%8==0)
//		{
//			Turn1 = 1;									//电机正转
//		}
//		else
//			Turn1 = 0;
//		PWM_Out(pwm);									//输出pwm
//		
//		if(TIM5CH2_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
//		{
//			temp=TIM5CH2_CAPTURE_STA&0X3F; 
//			temp*=0XFFFFFFFF;				//溢出时间总和
//			temp+=TIM5CH2_CAPTURE_VAL;	    //得到总的高电平时间
//			speed = 3500/(temp/150);
//			if(Turn1==0)
//			{
//				speed = -speed;
//			}
//			printf(" No.%4d      pwm:%3d	High:%8d ms	Speed:%8d\r\n",t, pwm, temp, speed);//打印总的高点平时间
//			t++;
//			TIM5CH2_CAPTURE_STA=0;			//开启下一次捕获
//		}
////		i++;
////		if(i==200)
////		{
////			printf("                                                                   working PWM = %d\r\n",pwm);
////			i=0;
////		}
//		sprintf((char*)string1,"%5d",speed);
//		sprintf((char*)string2,"%5d",pwm);
//		sprintf((char*)string3,"%5d",num);
//		LCD_ShowString(140,40,200,16,16,string1);
//		LCD_ShowString(140,60,200,16,16,string2);
//		LCD_ShowString(140,80,200,16,16,string3);
//	}
//}

