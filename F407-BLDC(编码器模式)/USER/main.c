/*******************************************************
��stm32F4��������,ͨ��KEY2��KEY3����pwmռ�ձȵ����������,
�Ӷ�����PF8��PWM��pwm���(���Ը�)
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
	u8 T = 0;											//��������
	s8 pwm = 20;											//pwmֵ
//	s16 num=0;
//	u8 string1[5];										//��ʾ�ַ���
//	u8 string2[5];										//��ʾ�ַ���
//	u8 string3[5];
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����ϵͳ�ж����ȼ�����2
	delay_init(168);  									//��ʼ����ʱ����
	uart_init(115200);									//��ʼ�����ڲ�����Ϊ115200
	TIM8_PWM_Init(500-1,84-1);							//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
	Motor_Turn_Init();									//���ת����Ƴ�ʼ��
	Key_GPIO_Config();									//������ʼ��
//	Timer5_Encoder_Init();								//��������ʼ��
	Capture_Init();
//	Timer3_Init(5000-1,8400-1);							//100ms�ж�һ��
	
	
//	//LCD��Ļ��ʾ����
//	LCD_Init();							//LCD��Ļ��ʼ��
//	POINT_COLOR=GRED;					//������ɫ
//	LCD_Clear(BLACK);					//������ɫΪ��ɫ
//	LCD_ShowString(60,10,200,24,24,"BalanceCar");
//	LCD_ShowString(50,40,200,16,16,"Speed:");
//	LCD_ShowString(50,60,200,16,16,"PWM  :");
//	LCD_ShowString(50,80,200,16,16,"text :");
	
	while(1)
	{ 
		T = Key_Scan(0);								//ɨ�谴��
		if(T == 2)	    pwm = pwm + 4;
		else if(T == 3) pwm = pwm - 4;
		if(pwm > 70)	pwm = 0;
		if(pwm <= 0)	pwm = 0;
		if(pwm%8==0)
		{
			Turn1 = 1;									//�����ת
		}
		else
			Turn1 = 0;
		PWM_Out(pwm);									//���pwm

//		sprintf((char*)string1,"%5d",num);
//		sprintf((char*)string2,"%5d",pwm);
//		sprintf((char*)string3,"%5d",num);
//		LCD_ShowString(140,40,200,16,16,string1);
//		LCD_ShowString(140,60,200,16,16,string2);
//		LCD_ShowString(140,80,200,16,16,string3);
	}
}

