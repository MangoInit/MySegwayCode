#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
 /*
 * ��������Key_GPIO_Config
 * ����  �����ð����õ���I/O��
 * ����  ����
 * ���  ����
 */
void Key_GPIO_Config(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 ��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
}
/******************************
����: ��������
����: 0,��֧����������;1,֧����������
����: 0,û���κΰ�������
		   2,KEY2������
		   3,KEY3������
��ע: �˺�������Ӧ���ȼ�,KEY0>WAK UP,�� KEY3>KEY2
******************************/
u8 Key_Scan(u8 mode)
{
	static u8 key_up = 1;			//�����ɿ���־
	if(mode) key_up = 1;			//֧������
	if(key_up&&(KEY2==1||KEY3==0))
	{
		delay_ms(15);						//ȥ����
		key_up = 0;
		if(KEY2==1)
		{
			delay_ms(15);						//ȥ����
			return 2;
		}
		else if(KEY3==0)
		{
			delay_ms(15);						//ȥ����
			return 3;
		}
	}
	else if(KEY2==0&&KEY3==1)
	{
		delay_ms(15);						//ȥ����
		key_up = 1;
	}
	return 0;
}

