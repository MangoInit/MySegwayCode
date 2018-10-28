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
//ALIENTEKս��STM32������ ʵ��0
//�½����� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
int main(void)
{				 
	u8 t=0; 
	Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);	  		//��ʱ��ʼ��
	uart_init(72,9600); 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72); 	//��ʼ��USMART	
  	while(1)
	{
		printf("t:%d\r",t);
		printf("kk = %d \r\t\r mm = %d \r\n",kk,mm);
		delay_ms(500);
		t++;
	}	 
} 






