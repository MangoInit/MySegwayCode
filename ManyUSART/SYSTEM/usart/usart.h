#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 

 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART USART1		//������ʹ�õĴ���
//�������ʹ��USART1����Ҫ�޸��жϺ�������USART1_IRQHandler
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u8 tx_num;		//�������ݵ��ֽ���
extern u8 *tx_p;		//�������ݵĻ�����ָ��
extern u8 tx_index;		//�������ݵ�����

void uart_init(u32 pclk2,u32 bound);
void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void uart4_init(u32 bound);
void uart5_init(u32 bound);
void usart_transmit(void* buf,u32 n);

#endif	   
















