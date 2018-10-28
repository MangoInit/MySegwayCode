#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 

 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART USART1		//定义所使用的串口
//如果不是使用USART1，需要修改中断函数名称USART1_IRQHandler
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u8 tx_num;		//发送数据的字节数
extern u8 *tx_p;		//发送数据的缓冲区指针
extern u8 tx_index;		//发送数据的索引

void uart_init(u32 pclk2,u32 bound);
void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void uart4_init(u32 bound);
void uart5_init(u32 bound);
void usart_transmit(void* buf,u32 n);

#endif	   
















