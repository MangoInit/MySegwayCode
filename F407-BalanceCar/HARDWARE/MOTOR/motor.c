#include "motor.h"
//TIM1_1/4 PWM���ֳ�ʼ��
//PWM�����ʼ��(PA8/PA11)
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
void Motor_PWM_Init(u32 arr,u32 psc)
{
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 | GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA8/11����Ϊ��ʱ��1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;	//GPIO8/11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//��ʼ��PA8
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;					//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;						//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//ʱ�ӷָ�Ϊ0
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	//��ʼ��TIM1 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_Pulse = 0;								//���ô�װ�벶��ȽϼĴ���������ֵ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//�������:TIM����Ƚϼ��Ը�***
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//����Tָ���Ĳ�����ʼ������TIM1 OC1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);				//ʹ��TIM1��ͨ��1 CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1,ENABLE);		//ARPEʹ�� 
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);		//�߼���ʱ����Ҫ
	TIM_Cmd(TIM1, ENABLE);  				//ʹ��TIM1				  
}

//TIM1 PWM���ռ�ձ�
//PWM���޸�ռ�ձ�(TIM1<-CRR1)
void PWM_Out(u16 PWM1, u16 PWM2)
{
	TIM_SetCompare1(TIM1, PWM1);		//ͨ��1
	TIM_SetCompare4(TIM1, PWM2);		//ͨ��4
}

//����ת���Ƴ�ʼ��
//����4.7K����������0V��5V��ѹ(PA2/PA3)
void Motor_Turn_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB 
}












