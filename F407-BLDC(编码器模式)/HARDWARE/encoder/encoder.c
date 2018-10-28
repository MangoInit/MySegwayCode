#include "encoder.h"

#define ENCODER_PPR           (u16)(400)   // number of pulses per revolution

u16 encoder;								//����

/**********************************************
�������ܣ���TIM5��ʼ��Ϊ�������ӿ���ʽ

**********************************************/
void Timer5_Encoder_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);			//TIM5ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���޸�*
//	TIM_TimeBaseStructure.TIM_Period = ENCODER_PPR*2 - 1;		//����������*2Ϊ���ֵ
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;					//����Ƶ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	//������ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;						//ʹ���˲�10
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);						//������б�־λ
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);					//�����жϸ���
    TIM5->CNT = 0;												//������0
	//TIM_SetCounter(TIM5,0);									//������0
	TIM_Cmd(TIM5, ENABLE);										//����TIM5��ʱ��
	printf("ok\r\n");
}

/************************************************
�������ܣ�TIM5�жϷ�����

************************************************/
void TIM5_IRQHandler(void)
{
//	if(TIM4->SR&0X0001)				//����ж�
//	{
//		
//	}
//	TIM4->SR&=~(1<<0);				//����жϱ�־λ
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	if(encoder != 65535)
	{
		encoder++;
	}
}

/************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
�� �� ֵ���ٶ�ֵ
************************************************/
u32 Read_Encoder(u8 TIMX)
{
	u32 Encoder_TIM;
	switch(TIMX)
	{
		case 5:		Encoder_TIM = TIM5->CNT;	TIM5->CNT = 0;	break;
		default:	Encoder_TIM = 0;
	}
	return Encoder_TIM;
}


//void Timer2_Encoder_Init()
//{
//	
//}

