#include "motor.h"
//���ҵ������ת��־λ
uint8_t Motor_Left,Motor_Right;

//��������ת����
//1����ת
//0����ת
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


//�ҵ������ת����
//1����ת
//0����ת
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

