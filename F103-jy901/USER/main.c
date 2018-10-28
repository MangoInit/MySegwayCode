#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "jy901.h"
#include "usart.h"
#include "delay.h"
#include "IOI2C.h"

int main(void)
{  

	uart_init(115200);
	delay_init();
	IIC_Init();
	
	while (1)
	{
		delay_ms(500);
		jy901();
	}
}

