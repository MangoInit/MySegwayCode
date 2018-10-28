#include "pwm.h"

//TIM8 PWM���ֳ�ʼ��
//PWM�����ʼ��(PC6)
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u32 arr,u32 psc)
{
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTCʱ��	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOC6����Ϊ��ʱ��8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIO6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PC6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//ʱ�ӷָ�Ϊ0
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//��ʼ����ʱ��8
	
	//��ʼ��TIM8 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							 //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_Pulse = 0;														 //���ô�װ�벶��ȽϼĴ���������ֵ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	 		 //�������:TIM����Ƚϼ��Ը�***
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  										 //����Tָ���Ĳ�����ʼ������TIM8 OC1

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 						 //ʹ��TIM8��ͨ��1 CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8				  
}

//TIM8 PWM���ռ�ձ�
//PWM���޸�ռ�ձ�(TIM8<-CRR1)
void PWM_Out(u16 PWM)
{
	TIM_SetCompare1(TIM8, PWM);		//ͨ��1
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
