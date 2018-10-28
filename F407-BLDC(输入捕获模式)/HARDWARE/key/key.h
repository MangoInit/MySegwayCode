#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

/*通过直接操作库函数方式读取IO*/
#define KEY3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

/*通过位带操作方式读取IO*/
/*
#define KEY3 PEin(4)
#define KEY2 PAin(0)
*/

void Key_GPIO_Config(void);
u8 Key_Scan(u8 mode);
#endif
