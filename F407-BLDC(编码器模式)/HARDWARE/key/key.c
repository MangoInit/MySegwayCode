#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
 /*
 * 函数名：Key_GPIO_Config
 * 描述  ：配置按键用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void Key_GPIO_Config(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
}
/******************************
功能: 按键处理
输入: 0,不支持连续按键;1,支持连续按键
返回: 0,没有任何按键按下
		   2,KEY2被按下
		   3,KEY3被按下
备注: 此函数有响应优先级,KEY0>WAK UP,即 KEY3>KEY2
******************************/
u8 Key_Scan(u8 mode)
{
	static u8 key_up = 1;			//按键松开标志
	if(mode) key_up = 1;			//支持连按
	if(key_up&&(KEY2==1||KEY3==0))
	{
		delay_ms(15);						//去抖动
		key_up = 0;
		if(KEY2==1)
		{
			delay_ms(15);						//去抖动
			return 2;
		}
		else if(KEY3==0)
		{
			delay_ms(15);						//去抖动
			return 3;
		}
	}
	else if(KEY2==0&&KEY3==1)
	{
		delay_ms(15);						//去抖动
		key_up = 1;
	}
	return 0;
}

