#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "usmart.h"
u16 kk = 1,mm = 2;
void PID(u16 p,u16 d)
{
	kk = p;
	mm = d;
}
//ALIENTEK战舰STM32开发板 实验0
//新建工程 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
int main(void)
{				 
	u8 t=0; 
	Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);	  		//延时初始化
	uart_init(72,9600); 	//串口初始化为115200
	usmart_dev.init(72); 	//初始化USMART	
  	while(1)
	{
		printf("t:%d\r",t);
		printf("kk = %d \r\t\r mm = %d \r\n",kk,mm);
		delay_ms(500);
		t++;
	}	 
} 






