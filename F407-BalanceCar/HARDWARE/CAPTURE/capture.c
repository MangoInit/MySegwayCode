#include "capture.h"
//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 

//定时器5通道1输入捕获配置(32位)
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 	//使能PORTA时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOH10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOH,&GPIO_InitStructure); //初始化PH10

	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //PH10复用位定时器5

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);


	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ITConfig(TIM5,TIM_IT_Update | TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断	

	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}

//捕获状态
//[7]:  0,没有成功的捕获;1,成功捕获到一次.
//[6]:  0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 		    

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}
				else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//获取当前的捕获值.
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器5
	 			TIM_SetCounter(TIM5,0);
	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
			}		    
		}			     	     					   
 	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}



//定时器2通道3输入捕获配置(32位)
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM2_CH3_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); //PB10复用位定时器2

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);


	//初始化TIM2输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI3上
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update | TIM_IT_CC3, ENABLE);//允许更新中断 ,允许CC1IE捕获中断	

	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}

//捕获状态
//[7]:  0,没有成功的捕获;1,成功捕获到一次.
//[6]:  0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM2CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH3_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    

 	if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM2CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM2CH3_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM2CH3_CAPTURE_VAL=0XFFFFFFFF;
				}
				else TIM2CH3_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)//捕获3发生捕获事件
		{	
			if(TIM2CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM2CH3_CAPTURE_VAL=TIM_GetCapture3(TIM2);//获取当前的捕获值.
				TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC3P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM2CH3_CAPTURE_STA=0;			//清空
				TIM2CH3_CAPTURE_VAL=0;
				TIM2CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM2,DISABLE ); 	//关闭定时器2
	 			TIM_SetCounter(TIM2,0);
	 			TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC3P=1 设置为下降沿捕获
				TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
			}
		}			     	     					   
 	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
}

//读取脉冲数据,并进行转换
//入口参数：定时器(2/5)
//返 回 值：速度值
u32 Read_Speed(u8 TIMX)
{
	u32 temp=0;
	if (TIMX == 5)
	{
		if(TIM5CH1_CAPTURE_STA&0X80)			//成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=0XFFFFFFFF;					//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;			//得到总的高电平时间
//			printf("HIGH:%d us\r\n",temp);		//打印总的高点平时间	long long temp = 0  %lld
			TIM5CH1_CAPTURE_STA=0;				//开启下一次捕获
		}
	}
	else if (TIMX == 2)
	{
		if(TIM2CH3_CAPTURE_STA&0X80)			//成功捕获到了一次高电平
		{
			temp=TIM2CH3_CAPTURE_STA&0X3F;
			temp*=0XFFFFFFFF;					//溢出时间总和
			temp+=TIM2CH3_CAPTURE_VAL;			//得到总的高电平时间
//			printf("HIGH:%d us\r\n",temp);		//打印总的高点平时间	long long temp = 0  %lld
			TIM2CH3_CAPTURE_STA=0;				//开启下一次捕获
		}
	}
	else
		return 0;
	return temp;
}
