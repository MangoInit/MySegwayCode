#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#include "delay.h"

#define Beep PFout(8)	// ����������IO 

void Beep_Init(void);	//��ʼ��
void Beep_Start(void);	//��ʼ����
#endif
