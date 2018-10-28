/*
    程序修改自惠特例程C51例程
    作者：gentalk
    邮箱：gentalk@163.com
    修改过的程序已在STM32,430,DSP28335,C51上测试过，比原程序在时序上的兼容性更好。
    在不同平台上的移植要注意codetab.h中数组的定义，oled.c中OLED_GPIO_Init()对IO口初始化的移植，以及oled.h中接口的定义。
*/

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"

int main()
{
//  unsigned char i;
  delay_init(); //延时初始化
  uart_init(9600);  //串口通信初始化
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
//				for(i=0; i<8; i++)//通过点整显示汉字 -- i表示字表数组的位置			//有误
//				{
//					OLED_P16x16Ch(i*16,0,i);
//				 	OLED_P16x16Ch(i*16,2,i+8);
//				 	OLED_P16x16Ch(i*16,4,i+16);
//				 	OLED_P16x16Ch(i*16,6,i+24);
//				}
				delay_ms(1000);
        delay_ms(1000);
//				OLED_CLS();//清屏
//				OLED_P8x16Str(0,0,"HelTec");//第一行 -- 8x16的显示单元显示ASCII码
//				OLED_P8x16Str(0,2,"OLED Display");
//				OLED_P8x16Str(0,4,"www.heltec.cn");
//				OLED_P6x8Str(0,6,"cn.heltec@gmail.com");
//				OLED_P6x8Str(0,7,"heltec.taobao.com");
				OLED_CLS();
				Draw_BMP(0,0,128,8,BMP2);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
				while(1)
        delay_ms(1000);
	}   

}
