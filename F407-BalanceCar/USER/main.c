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
	u8 lcd_string_1[5];				//�����ʾ�ַ���
	u8 lcd_string_2[5];				//�����ʾ�ַ���
	u8 lcd_string_3[5];				//�����ʾ�ַ���
	u8 lcd_string_4[5];
	u8 lcd_string_5[5];
	u8 lcd_string_6[5];
	
	delay_init(168);					//��ʱ��ʼ��
	uart_init(115200);				//���ڳ�ʼ��
	IIC_Init();								//IIC��ʼ��
	Beep_Init();						//��������ʼ��
	Motor_PWM_Init(500-1,84-1);					//PWM�����ʼ��,84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
	Motor_Turn_Init();									//�������˳�ʼ��
	TIM5_CH1_Cap_Init(0xFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ���,
	TIM2_CH3_Cap_Init(0xFFFFFFFF,84-1);	//��·�������벶���ʼ��
	Turn_Adc_Init();										//ת����ADC��ʼ��
	Timer_Init(50-1,8400-1);						//5ms�ж�һ��
	
	LCD_Init();								//LCD��Ļ��ʼ��
	POINT_COLOR=GRED;					//������ɫ
	LCD_Clear(BLACK);					//������ɫΪ��ɫ
	LCD_ShowString(60,10,200,24,24,"BalanceCar");
	LCD_ShowString(20,40,200,16,16,"Angle Balance:");
	LCD_ShowString(20,60,200,16,16,"Gyro Balance :");
	LCD_ShowString(20,80,200,16,16,"Gyro Turn    :");
	LCD_ShowString(20,120,200,16,16,"Balance Pwm  :");
	LCD_ShowString(20,140,200,16,16,"Velocity Pwm :");
	LCD_ShowString(20,160,200,16,16,"Turn Pwm     :");

//	Beep_Start();						//������ʼ�����������
	Steer_Init();						//������¼ת����ֵĳ�ʼλ��
	
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

