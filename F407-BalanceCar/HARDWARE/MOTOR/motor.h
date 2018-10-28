#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

//�������ת����,0/1����
#define Turn_Motor_1 PA(2)
#define Turn_Motor_2 PA(3)

//��·PWM��ֵ�ڶ�ʱ��ͨ����
#define PWM_1 TIM1->CRR1
#define PWM_2 TIM2->CRR4

void Motor_PWM_Init(u32 arr,u32 psc);
void PWM_Out(u16 PWM1, u16 PWM2);
void Motor_Turn_Init(void);


#endif
