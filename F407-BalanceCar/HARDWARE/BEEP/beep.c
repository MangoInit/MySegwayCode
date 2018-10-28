#include "beep.h"
//��ʼ��PF8Ϊ�����		    
//BEEP IO��ʼ��
void Beep_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
  
  //��ʼ����������Ӧ����GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����(������Ϊ�͵�ƽ����)
  GPIO_Init(GPIOF, &GPIO_InitStructure);			//��ʼ��GPIO
	
  GPIO_ResetBits(GPIOF,GPIO_Pin_8); 	 //��������Ӧ����GPIOF8���ͣ� 
}

void Beep_Start(void)					//������
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);	//����
	delay_ms(50);
	GPIO_SetBits(GPIOF,GPIO_Pin_8);		//����
	delay_ms(100);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);	//����
	delay_ms(50);
	GPIO_SetBits(GPIOF,GPIO_Pin_8);		//����
	delay_ms(100);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);	//����
	delay_ms(50);
	GPIO_SetBits(GPIOF,GPIO_Pin_8);		//����
	delay_ms(100);
}
