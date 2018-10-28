#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h" 
#include "key.h"	 	 


int main(void)
{								  
	char temp[]="Hello World!\n";
	
	Stm32_Clock_Init(9);	//系统时钟设置
	usart1_init(115200); 	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口 

 	while(1)
	{
		usart_transmit(temp,sizeof(temp));
		LED0=!LED0;
		delay_ms(1000);   
	}		 
}

