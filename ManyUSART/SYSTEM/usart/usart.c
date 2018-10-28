#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

//�����жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  


//�������ܣ��������ݴ����USART_RX_BUF��Ȼ��ͨ���жϷ��͵ķ�ʽ���ؽ��յ�����
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif

//��������	
	if((USART->CR1&1<<6)!=0)	//��������˴��ڷ����ж�
	{
		USART->SR&=~(1<<6);		//״̬�Ĵ���������ɱ�־λ����
		if(tx_index<tx_num)	//������������д���������
		{
			USART->DR=*(tx_p+tx_index);	//��������
			tx_index++;
		}
		else	//������ݷ������
		{
			USART->CR1&=~(1<<6);		//�رշ�������ж�
			tx_index=0;
		}		
	}

//��������	
	if(USART->SR&(1<<5))	//���յ�����
	{	 
		res=USART->DR; 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	}

//���ؽ��յ�������	
	if(USART_RX_STA&0x8000)
	{	
		usart_transmit(USART_RX_BUF,USART_RX_STA&0X3FFF);
		USART_RX_STA=0;
	}
	
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
									 
//��ʼ��IO ����1
//bound:������ 
void usart1_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(72*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<14;  //ʹ��USART1ʱ��		
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  	
	GPIOA->CRH&=0XFFFFF00F;	
	GPIOA->CRH|=0X000008B0;	//PA9�������������PA10����/��������
	RCC->APB2RSTR|=1<<14;   //��λUSART1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   			
   
	//����������
 	USART->BRR=mantissa; // ����������	 
	USART->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
}

void usart2_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<17;  //ʹ��USART2ʱ��		
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  	
	GPIOA->CRL&=0XFFFF00FF;	
	GPIOA->CRL|=0X00008B00;	//PA2�������������PA3����/��������
	RCC->APB1RSTR|=1<<17;   //��λUSART2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   			
   
	//����������
 	USART->BRR=mantissa; // ����������	 
	USART->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
	MY_NVIC_Init(3,3,USART2_IRQn,2);//��2��������ȼ� 
}

void usart3_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 	   		

	RCC->APB1ENR|=1<<18;  //ʹ��USART3ʱ��		
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  	
	GPIOB->CRH&=0XFFFF00FF;	
	GPIOB->CRH|=0X00008B00;	//PB10�������������PB11����/��������
	RCC->APB1RSTR|=1<<18;   //��λUSART3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ			
   
	//����������
 	USART->BRR=mantissa; // ����������	 
	USART->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
	MY_NVIC_Init(3,3,USART3_IRQn,2);//��2��������ȼ� 
}

void uart4_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<19;  //ʹ��UART4ʱ��		
	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��  	
	GPIOC->CRH&=0XFFFF00FF;	
	GPIOC->CRH|=0X00008B00;	//PC10�������������PC11����/��������
	RCC->APB1RSTR|=1<<19;   //��λUART4
	RCC->APB1RSTR&=~(1<<19);//ֹͣ��λ	   			
   
	//����������
 	USART->BRR=mantissa; // ����������	 
	USART->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
	MY_NVIC_Init(3,3,UART4_IRQn,2);//��2��������ȼ� 
}

void uart5_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<20;  //ʹ��UART5ʱ��		
	RCC->APB2ENR|=3<<4;   //ʹ��PORTC\PORTD��ʱ��  	
	GPIOC->CRH&=0XFFF0FFFF;	
	GPIOC->CRH|=0X000B0000;	//PC12�������������PA3����/��������
	GPIOD->CRL&=0XFFFFF0FF;	
	GPIOD->CRL|=0X00000800;	//PD2����/��������
	RCC->APB1RSTR|=1<<20;   //��λUART5
	RCC->APB1RSTR&=~(1<<20);//ֹͣ��λ	   			
   
	//����������
 	USART->BRR=mantissa; // ����������	 
	USART->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
	MY_NVIC_Init(3,3,UART5_IRQn,2);//��2��������ȼ� 
}


u8 tx_num=0;	//�������ݵ��ֽ���
u8 *tx_p;		//�������ݵĻ�����ָ��
u8 tx_index=0;	//�������ݵ�����

//�������ݣ�buf�����ݵ�ָ�룬nΪ���ݳ���
void usart_transmit(void* buf,u32 n)
{	
	tx_num=n;
	tx_p=(u8 *)buf;
	
	if((USART->CR1&1<<6)==0)	//�����������жϽ���
		USART->CR1|=1<<6;		//��������ж�ʹ��
	USART->DR = *tx_p;      	//дDR,���ڽ���������
	tx_index=1;
}

