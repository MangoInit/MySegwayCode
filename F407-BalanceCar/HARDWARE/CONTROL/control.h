#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "jy901.h"
#include "capture.h"
#include "motor.h"
#include "adc.h"
#define ZHONGZHI 0									//平衡车机械中值
#define ZF1 PAout(2)								//左电机转向
#define ZF2 PAout(3)								//右电机转向
extern int Left_Speed,Right_Speed;					//左右电机速度
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;	//平衡倾角 平衡陀螺仪 转向陀螺仪
extern u32 Balance_Pwm,Velocity_Pwm,Turn_Pwm;		//各个环的PWM

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
