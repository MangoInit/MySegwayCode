#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "jy901.h"
#include "capture.h"
#include "motor.h"
#include "adc.h"
#define ZHONGZHI 0									//ƽ�⳵��е��ֵ
#define ZF1 PAout(2)								//����ת��
#define ZF2 PAout(3)								//�ҵ��ת��
extern int Left_Speed,Right_Speed;					//���ҵ���ٶ�
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;	//ƽ����� ƽ�������� ת��������
extern u32 Balance_Pwm,Velocity_Pwm,Turn_Pwm;		//��������PWM

int Balance_Control(float Angle,float Gyro);
int Velocity_Control(int Left_Speed, int Right_Speed);
//int Turn_Control(int Left_Speed, int Right_Speed, float gyro);
int Turn_Control(void);

int Control(void);
int Myabs(int a);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle);
void Set_PWM(s32 moto1,s32 moto2);
#endif
