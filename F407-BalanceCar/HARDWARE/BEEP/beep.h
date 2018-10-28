#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#include "delay.h"

#define Beep PFout(8)	// 蜂鸣器控制IO 

void Beep_Init(void);	//初始化
void Beep_Start(void);	//开始工作
#endif
