#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f1xx_hal.h"
#include "main.h"
#include "jy901.h"
#include "tim.h"
#include "adc.h"

extern float ZHONGZHI;															//机械中值

extern float Angle_Balance,Gyro_Balance,Gyro_Turn;	//平衡倾角 平衡陀螺仪 转向陀螺仪
extern uint16_t JY901_Temperature;										//陀螺仪温度
extern int16_t Left_Speed, Right_Speed;							//左右电机速度
extern int16_t Balance_PWM, Velocity_PWM, Turn_PWM;	//平衡环，速度还，转向环的PWM
extern int16_t Moto1, Moto2;												//电机PWM变量
extern float	Balance_PD_P, Balance_PD_D;						//直立环PD参数
extern float Velocity_PI_P, Velocity_PI_I;					//速度环PI参数
extern uint8_t Danger_Flag;													//停止PWM输出运行标志
extern uint8_t Stop_Flag, Qian_Flag,Hou_Flag,Left_Flag,Right_Flag;//蓝牙控制变量

int All_Control(void);

int Balance_Control(float Angle,float Gyro);
int Velocity_Control(int Left_Speed, int Right_Speed);
int Turn_Control_Bluetooth_Mode(int Left_Speed, int Right_Speed, float gyro);
int Turn_Control_Manned_Mode(float Gyro_Turn);

int Control(void);
int Myabs(int a);
void Xianfu_PWM(void);
uint8_t Turn_Off(float angle);
void Set_PWM(int32_t moto1,int32_t moto2);
#endif
