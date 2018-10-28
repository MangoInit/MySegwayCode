#include "motor.h"
//左右电机正反转标志位
uint8_t Motor_Left,Motor_Right;

//左电机正反转控制
//1：正转
//0：反转
void Motor_Turn_Left(uint8_t Direction)
{
	if(Direction == 1)
	{
		HAL_GPIO_WritePin(Motor_Turn_Left_GPIO_Port, Motor_Turn_Left_Pin, GPIO_PIN_SET);
		Motor_Left = 1;
	}
	else if(Direction == 0)
	{
		HAL_GPIO_WritePin(Motor_Turn_Left_GPIO_Port, Motor_Turn_Left_Pin, GPIO_PIN_RESET);
		Motor_Left = 0;
	}
}


//右电机正反转控制
//1：正转
//0：反转
void Motor_Turn_Right(uint8_t Direction)
{
	if(Direction == 1)
	{
		HAL_GPIO_WritePin(Motor_Turn_Right_GPIO_Port, Motor_Turn_Right_Pin, GPIO_PIN_SET);
		Motor_Right  = 1;
	}
	else if(Direction == 0)
	{
		HAL_GPIO_WritePin(Motor_Turn_Right_GPIO_Port, Motor_Turn_Right_Pin, GPIO_PIN_RESET);
		Motor_Right = 0;
	}
}

