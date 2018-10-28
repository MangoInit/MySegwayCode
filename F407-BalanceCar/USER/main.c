#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "jy901.h"
#include "iic.h"
#include "beep.h"
#include "capture.h"
#include "timer.h"
#include "lcd.h"

int main(void)
{  
	u8 lcd_string_1[5];				//存放显示字符串
	u8 lcd_string_2[5];				//存放显示字符串
	u8 lcd_string_3[5];				//存放显示字符串
	u8 lcd_string_4[5];
	u8 lcd_string_5[5];
	u8 lcd_string_6[5];
	
	delay_init(168);					//延时初始化
	uart_init(115200);				//串口初始化
	IIC_Init();								//IIC初始化
	Beep_Init();						//蜂鸣器初始化
	Motor_PWM_Init(500-1,84-1);					//PWM输出初始化,84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
	Motor_Turn_Init();									//电机换向端初始化
	TIM5_CH1_Cap_Init(0xFFFFFFFF,84-1); //以1Mhz的频率计数,
	TIM2_CH3_Cap_Init(0xFFFFFFFF,84-1);	//两路测速输入捕获初始化
	Turn_Adc_Init();										//转向轴ADC初始化
	Timer_Init(50-1,8400-1);						//5ms中断一次
	
	LCD_Init();								//LCD屏幕初始化
	POINT_COLOR=GRED;					//画笔颜色
	LCD_Clear(BLACK);					//背景颜色为黑色
	LCD_ShowString(60,10,200,24,24,"BalanceCar");
	LCD_ShowString(20,40,200,16,16,"Angle Balance:");
	LCD_ShowString(20,60,200,16,16,"Gyro Balance :");
	LCD_ShowString(20,80,200,16,16,"Gyro Turn    :");
	LCD_ShowString(20,120,200,16,16,"Balance Pwm  :");
	LCD_ShowString(20,140,200,16,16,"Velocity Pwm :");
	LCD_ShowString(20,160,200,16,16,"Turn Pwm     :");

//	Beep_Start();						//开机初始化完成响三声
	Steer_Init();						//开机记录转向把手的初始位置
	
	while (1)
	{
		sprintf((char*)lcd_string_1,"%7.3f",Angle_Balance);
		sprintf((char*)lcd_string_2,"%7.3f",Gyro_Balance);
		sprintf((char*)lcd_string_3,"%7.3f",Gyro_Turn);
		sprintf((char*)lcd_string_4,"%5d",Balance_Pwm);
		sprintf((char*)lcd_string_5,"%5d",Velocity_Pwm);
		sprintf((char*)lcd_string_6,"%5d",Steer_Value);
		
		LCD_ShowString(140,40,200,16,16,lcd_string_1);
		
		LCD_ShowString(140,60,200,16,16,lcd_string_2);
		
		LCD_ShowString(140,80,200,16,16,lcd_string_3);
		
		LCD_ShowString(140,120,200,16,16,lcd_string_4);
		
		LCD_ShowString(140,140,200,16,16,lcd_string_5);
		
		LCD_ShowString(140,160,200,16,16,lcd_string_6);

		
	}
}

