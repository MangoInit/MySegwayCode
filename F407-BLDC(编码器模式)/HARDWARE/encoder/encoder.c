#include "encoder.h"

#define ENCODER_PPR           (u16)(400)   // number of pulses per revolution

u16 encoder;								//计数

/**********************************************
函数功能：把TIM5初始化为编码器接口形式

**********************************************/
void Timer5_Encoder_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);			//TIM5时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//使能PORTA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//可修改*
//	TIM_TimeBaseStructure.TIM_Period = ENCODER_PPR*2 - 1;		//编码器线数*2为最大值
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;					//不分频
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	//编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;						//使用滤波10
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);						//清除所有标志位
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);					//允许中断更新
    TIM5->CNT = 0;												//计数置0
	//TIM_SetCounter(TIM5,0);									//计数置0
	TIM_Cmd(TIM5, ENABLE);										//启动TIM5定时器
	printf("ok\r\n");
}

/************************************************
函数功能：TIM5中断服务函数

************************************************/
void TIM5_IRQHandler(void)
{
//	if(TIM4->SR&0X0001)				//溢出中断
//	{
//		
//	}
//	TIM4->SR&=~(1<<0);				//清除中断标志位
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	if(encoder != 65535)
	{
		encoder++;
	}
}

/************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返 回 值：速度值
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

