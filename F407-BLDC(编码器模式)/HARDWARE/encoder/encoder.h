#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#include "usart.h"

void Timer5_Encoder_Init(void);
u32 Read_Encoder(u8 TIMX);


extern u16 encoder;								//¼ÆÊý

#endif
