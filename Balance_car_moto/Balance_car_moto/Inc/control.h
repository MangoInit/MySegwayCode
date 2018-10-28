#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f1xx_hal.h"
#include "main.h"
#include "jy901.h"
#include "tim.h"
#include "adc.h"

extern float ZHONGZHI;															//��е��ֵ

extern float Angle_Balance,Gyro_Balance,Gyro_Turn;	//ƽ����� ƽ�������� ת��������
extern uint16_t JY901_Temperature;										//�������¶�
extern int16_t Left_Speed, Right_Speed;							//���ҵ���ٶ�
extern int16_t Balance_PWM, Velocity_PWM, Turn_PWM;	//ƽ�⻷���ٶȻ���ת�򻷵�PWM
extern int16_t Moto1, Moto2;												//���PWM����
extern float	Balance_PD_P, Balance_PD_D;						//ֱ����PD����
extern float Velocity_PI_P, Velocity_PI_I;					//�ٶȻ�PI����
extern uint8_t Danger_Flag;													//ֹͣPWM������б�־
extern uint8_t Stop_Flag, Qian_Flag,Hou_Flag,Left_Flag,Right_Flag;//�������Ʊ���

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
