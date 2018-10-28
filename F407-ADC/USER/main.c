/*************************************
���ܣ�ƽ�⳵ת����ADC����ץȡ
��ע��ÿ������ǰת�����λ��Ϊ��ʼλ��
���ԣ��ֻ��������ڵ��ԣ�������9600
ʱ�䣺2017��10��20��
���ߣ�Mango
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
	u16 Init_ADC_Value;															//�з���ԭʼADC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����ϵͳ�ж����ȼ�����2
	delay_init(168);  															//��ʼ����ʱ����
	uart_init(9600);																//��ʼ�����ڲ�����Ϊ115200
	LED_Init();
	Adc_Init();
//	TIM9_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
//	PWM_Out(1);					//0-499
	Init_ADC_Value = (uc16)Get_Adc_Average(ADC_Channel_1,20);				//��ʼ��λ��
	while(1)
	{
//		LED_Blink();
		u16 adcx;
		s16 adcx_treated;
		float temp;
		u8 t;
		adcx = Get_Adc_Average(ADC_Channel_1,20);					//��ȡͨ��1��ת��ֵ��20��ȡƽ��
		adcx_treated = (s16)adcx - (u16)Init_ADC_Value;				//��ת���ɼ�����(˳ʱ��Ϊ������ʱ��Ϊ��)
		temp = (float)adcx*(3.3/4096);								//��ʾADC�������ԭʼֵ��2^12λ)
//		adcx = temp;												//��ֵ�������֣�adcxΪu16λ����
//		temp = temp - adcx;											//С������
//		temp = temp * 1000;											//������λС��
		delay_ms(10);
		LED0=!LED0;
		LED1=!LED1;
		printf("%3d:ADC = %4d		turn = %5d			Init_ADC = %d\r\n",t, adcx, adcx_treated, Init_ADC_Value);
		t++;
	}
}

