#include "led.h"
#include "delay.h"

//��ʼ��PF9��PF10Ϊ����ڣ���ʹ��ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);		//ʹ��GPIOFʱ��
	
	//GPIOF9/10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOF,&GPIO_InitStructure);		//��ʼ��
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);	//GPIOF9/10���øߣ�����
}
void LED_Blink()		//LED��˸
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	delay_ms(500);
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	delay_ms(500);
}

