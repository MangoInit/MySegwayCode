#include "adc.h"

u16 Init_Steer_Value;			//初始转向轴的ADC数据
s16 Steer_Value;				//转向轴采集数据(正负之分)

//初始化ADC															   
void  Turn_Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

	//先初始化ADC1通道1 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//PA1 通道1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入(I/O口复用为ADC时模式设置为模拟输入，而不是复用功能)
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;	//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化  
	
	//校准
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);		//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);									//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);											//ADC初始化
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}

//设置ADC值
//ch：@reg ADC channels
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值：转换结果
u16 Get_Adc(u8 ch)
{
	//设置制定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

//获取通道ch的转换值，取times(20)次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}

//启动时记录转向轴的位置为初始位置
void Steer_Init()
{
	Init_Steer_Value = (uc16)Get_Adc_Average(ADC_Channel_1,20);
}

//转向轴数据采集
int Get_Steer()
{
	u16 adcx;					//原始ADC数据
	s16 adcx_treated;			//处理ADC数据
	float temp;					//转向显示数据
	adcx = Get_Adc_Average(ADC_Channel_1,20);			//获取通道1的转换值，20次取平均
	adcx_treated = (s16)adcx - (u16)Init_Steer_Value;	//轴转动采集数据(顺时针为正，逆时针为负)
	Steer_Value= adcx_treated;							//赋值
	temp = ((float)adcx_treated)/10;				//显示ADC采样后的原始值(2^12位)
	
	return temp;
}

