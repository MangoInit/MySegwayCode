#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "jy901.h"
#include "iic901.h"
int main(void)
{  
	delay_init(168);
	uart_init(115200);
	IIC_Init();
	
	while (1)
	{
		
		jy901();
		delay_ms(100);
	}
}

