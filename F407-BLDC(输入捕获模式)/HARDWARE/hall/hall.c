#include "hall.h"
#include "usart.h"
TIM_ICInitTypeDef  TIM5_ICInitStructure;

//��ʱ��5ͨ��2���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH2_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //PA1����λ��ʱ��5
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	

	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC2S=01 	ѡ������� IC2ӳ�䵽TI2��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI2��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC2IE�����ж�	
	
	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

	printf("capture init OK!\r\n");
	
	
}
//����״̬
//[7]:  0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:  0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
//u8  TIM5CH2_CAPTURE_STA=0;	//���벶��״̬		    				
//u32	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
//void TIM5_IRQHandler(void)
//{ 		    

// 	if((TIM5CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
//		{	     
//			if(TIM5CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					TIM5CH2_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
//					TIM5CH2_CAPTURE_VAL=0XFFFFFFFF;
//				}
//				else TIM5CH2_CAPTURE_STA++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//����2���������¼�
//		{	
//			if(TIM5CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  			
//				TIM5CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//				TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
//	 			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}
//			else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM5CH2_CAPTURE_STA=0;			//���
//				TIM5CH2_CAPTURE_VAL=0;
//				TIM5CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
//				TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��5
//	 			TIM_SetCounter(TIM5,0);
//	 			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
//				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
//}


u8  TIM5CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_CC2) != RESET)						//����1��������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2 | TIM_IT_Update);		//����жϱ�־λ
		if(TIM5CH2_CAPTURE_STA == 0)									//�����һ��������
		{
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);			//CC2P=1 ����Ϊ�½��ز���
			TIM5CH2_CAPTURE_STA = 1;									//��ǲ���һ���½���
			TIM5CH2_CAPTURE_VAL++;
		}
		else if(TIM5CH2_CAPTURE_STA == 1)
		{
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising);			//CC2P=1 ����Ϊ�½��ز���
			TIM5CH2_CAPTURE_STA = 0;									//��ǲ���һ���½���
			TIM5CH2_CAPTURE_VAL++;
		}
	}
}
/*
//��λʱ���ȡ��챵���ٶ�
int Read_Hall()
{
	int Hall;
	Hall = capture;
	return Hall;
}
*/

