#include "motor.h"
//TIM1_1/4 PWM部分初始化
//PWM输出初始化(PA8/PA11)
//arr:自动重装值
//psc:时钟预分频数
void Motor_PWM_Init(u32 arr,u32 psc)
{
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8 | GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA8/11复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;	//GPIO8/11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//初始化PA8
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;						//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//时钟分割为0
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM1 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高***
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//根据T指定的参数初始化外设TIM1 OC1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);				//使能TIM1在通道1 CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1,ENABLE);		//ARPE使能 
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);		//高级定时器需要
	TIM_Cmd(TIM1, ENABLE);  				//使能TIM1				  
}

//TIM1 PWM输出占空比
//PWM：修改占空比(TIM1<-CRR1)
void PWM_Out(u16 PWM1, u16 PWM2)
{
	TIM_SetCompare1(TIM1, PWM1);		//通道1
	TIM_SetCompare4(TIM1, PWM2);		//通道4
}

//正反转控制初始化
//上拉4.7K电阻控制输出0V和5V电压(PA2/PA3)
void Motor_Turn_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOB 
}












