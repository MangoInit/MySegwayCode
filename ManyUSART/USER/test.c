#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h" 
#include "key.h"	 	 


int main(void)
{								  
	char temp[]="Hello World!\n";
	
	Stm32_Clock_Init(9);	//ϵͳʱ������
	usart1_init(115200); 	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 

 	while(1)
	{
		usart_transmit(temp,sizeof(temp));
		LED0=!LED0;
		delay_ms(1000);   
	}		 
}

