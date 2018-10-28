#include "pwm.h"

//TIM9 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
void TIM9_PWM_Init(u32 arr,u32 psc)
{
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM9ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTEʱ��	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); //GPIOE5����Ϊ��ʱ��9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIOE5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//ʱ�ӷָ�Ϊ0
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//��ʼ����ʱ��9
	
	//��ʼ��TIM9 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							 //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_Pulse = 0;														 //���ô�װ�벶��ȽϼĴ���������ֵ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		 //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  										 //����Tָ���Ĳ�����ʼ������TIM9 OC1

	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable); 						 //ʹ��TIM9��ͨ��1 CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM9, ENABLE);  //ʹ��TIM9				  
}

//TIM9 PWM���ռ�ձ�
//PWM���޸�ռ�ձ�(TIM9<-CRR1)
void PWM_Out(u16 PWM)
{
	TIM_SetCompare1(TIM9, PWM);		//ͨ��1
}
