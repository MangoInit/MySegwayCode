#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  


//函数功能：接收数据存放在USART_RX_BUF，然后通过中断发送的方式返回接收的数据
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif

//发送数据	
	if((USART->CR1&1<<6)!=0)	//如果开启了串口发送中断
	{
		USART->SR&=~(1<<6);		//状态寄存器发送完成标志位清零
		if(tx_index<tx_num)	//如果缓冲区还有待发送数据
		{
			USART->DR=*(tx_p+tx_index);	//发送数据
			tx_index++;
		}
		else	//如果数据发送完成
		{
			USART->CR1&=~(1<<6);		//关闭发送完成中断
			tx_index=0;
		}		
	}

//接收数据	
	if(USART->SR&(1<<5))	//接收到数据
	{	 
		res=USART->DR; 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}

//返回接收到的数据	
	if(USART_RX_STA&0x8000)
	{	
		usart_transmit(USART_RX_BUF,USART_RX_STA&0X3FFF);
		USART_RX_STA=0;
	}
	
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
									 
//初始化IO 串口1
//bound:波特率 
void usart1_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(72*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<14;  //使能USART1时钟		
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  	
	GPIOA->CRH&=0XFFFFF00F;	
	GPIOA->CRH|=0X000008B0;	//PA9复用推挽输出，PA10上拉/下拉输入
	RCC->APB2RSTR|=1<<14;   //复位USART1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   			
   
	//波特率设置
 	USART->BRR=mantissa; // 波特率设置	 
	USART->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART->CR1|=1<<5;    //接收缓冲区非空中断使能
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
}

void usart2_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<17;  //使能USART2时钟		
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  	
	GPIOA->CRL&=0XFFFF00FF;	
	GPIOA->CRL|=0X00008B00;	//PA2复用推挽输出，PA3上拉/下拉输入
	RCC->APB1RSTR|=1<<17;   //复位USART2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   			
   
	//波特率设置
 	USART->BRR=mantissa; // 波特率设置	 
	USART->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART->CR1|=1<<5;    //接收缓冲区非空中断使能
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
}

void usart3_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 	   		

	RCC->APB1ENR|=1<<18;  //使能USART3时钟		
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  	
	GPIOB->CRH&=0XFFFF00FF;	
	GPIOB->CRH|=0X00008B00;	//PB10复用推挽输出，PB11上拉/下拉输入
	RCC->APB1RSTR|=1<<18;   //复位USART3
	RCC->APB1RSTR&=~(1<<18);//停止复位			
   
	//波特率设置
 	USART->BRR=mantissa; // 波特率设置	 
	USART->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART->CR1|=1<<5;    //接收缓冲区非空中断使能
	MY_NVIC_Init(3,3,USART3_IRQn,2);//组2，最低优先级 
}

void uart4_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<19;  //使能UART4时钟		
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  	
	GPIOC->CRH&=0XFFFF00FF;	
	GPIOC->CRH|=0X00008B00;	//PC10复用推挽输出，PC11上拉/下拉输入
	RCC->APB1RSTR|=1<<19;   //复位UART4
	RCC->APB1RSTR&=~(1<<19);//停止复位	   			
   
	//波特率设置
 	USART->BRR=mantissa; // 波特率设置	 
	USART->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART->CR1|=1<<5;    //接收缓冲区非空中断使能
	MY_NVIC_Init(3,3,UART4_IRQn,2);//组2，最低优先级 
}

void uart5_init(u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(36*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB1ENR|=1<<20;  //使能UART5时钟		
	RCC->APB2ENR|=3<<4;   //使能PORTC\PORTD口时钟  	
	GPIOC->CRH&=0XFFF0FFFF;	
	GPIOC->CRH|=0X000B0000;	//PC12复用推挽输出，PA3上拉/下拉输入
	GPIOD->CRL&=0XFFFFF0FF;	
	GPIOD->CRL|=0X00000800;	//PD2上拉/下拉输入
	RCC->APB1RSTR|=1<<20;   //复位UART5
	RCC->APB1RSTR&=~(1<<20);//停止复位	   			
   
	//波特率设置
 	USART->BRR=mantissa; // 波特率设置	 
	USART->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART->CR1|=1<<5;    //接收缓冲区非空中断使能
	MY_NVIC_Init(3,3,UART5_IRQn,2);//组2，最低优先级 
}


u8 tx_num=0;	//发送数据的字节数
u8 *tx_p;		//发送数据的缓冲区指针
u8 tx_index=0;	//发送数据的索引

//发送数据，buf是数据的指针，n为数据长度
void usart_transmit(void* buf,u32 n)
{	
	tx_num=n;
	tx_p=(u8 *)buf;
	
	if((USART->CR1&1<<6)==0)	//如果发送完成中断禁用
		USART->CR1|=1<<6;		//发送完成中断使能
	USART->DR = *tx_p;      	//写DR,串口将发送数据
	tx_index=1;
}

