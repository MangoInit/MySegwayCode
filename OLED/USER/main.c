/*
    �����޸��Ի�������C51����
    ���ߣ�gentalk
    ���䣺gentalk@163.com
    �޸Ĺ��ĳ�������STM32,430,DSP28335,C51�ϲ��Թ�����ԭ������ʱ���ϵļ����Ը��á�
    �ڲ�ͬƽ̨�ϵ���ֲҪע��codetab.h������Ķ��壬oled.c��OLED_GPIO_Init()��IO�ڳ�ʼ������ֲ���Լ�oled.h�нӿڵĶ��塣
*/

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"

int main()
{
//  unsigned char i;
  delay_init(); //��ʱ��ʼ��
  uart_init(9600);  //����ͨ�ų�ʼ��
  OLED_GPIO_Init();
  OLED_Init();
	OLED_CLS();
	while(1)
	{
				OLED_P16x16Ch(24,2,1);
				OLED_P16x16Ch(40,2,2);
				OLED_P16x16Ch(57,2,3);
				OLED_P16x16Ch(74,2,4);
				OLED_P16x16Ch(91,2,5);
//				for(i=0; i<8; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��			//����
//				{
//					OLED_P16x16Ch(i*16,0,i);
//				 	OLED_P16x16Ch(i*16,2,i+8);
//				 	OLED_P16x16Ch(i*16,4,i+16);
//				 	OLED_P16x16Ch(i*16,6,i+24);
//				}
				delay_ms(1000);
        delay_ms(1000);
//				OLED_CLS();//����
//				OLED_P8x16Str(0,0,"HelTec");//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
//				OLED_P8x16Str(0,2,"OLED Display");
//				OLED_P8x16Str(0,4,"www.heltec.cn");
//				OLED_P6x8Str(0,6,"cn.heltec@gmail.com");
//				OLED_P6x8Str(0,7,"heltec.taobao.com");
				OLED_CLS();
				Draw_BMP(0,0,128,8,BMP2);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
				while(1)
        delay_ms(1000);
	}   

}
