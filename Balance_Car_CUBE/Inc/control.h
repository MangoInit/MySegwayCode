#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f4xx_hal.h"
#include "main.h"
#include "jy901.h"
#include "tim.h"
#include "adc.h"

#define ZHONGZHI 0																		//机械中值

extern float Angle_Balance,Gyro_Balance,Gyro_Turn;		//平衡倾角 平衡陀螺仪 转向陀螺仪
extern int16_t Left_Speed, Right_Speed;								//左右电机速度
extern int16_t Balance_PWM, Velocity_PWM, Turn_PWM;	//平衡环，速度还，转向环的PWM


int All_Control(void);

int Balance_Control(float Angle,float Gyro);
int Velocity_Control(int Left_Speed, int Right_Speed);
//int Turn_Control(int Left_Speed, int Right_Speed, float gyro);
int Turn_Control(void);

int Control(void);
int Myabs(int a);
void Xianfu_PWM(void);
uint8_t Turn_Off(float angle);
void Set_PWM(int32_t moto1,int32_t moto2);

#endif
