#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_hal.h"
#include "main.h"

extern uint8_t Motor_Left,Motor_Right;

void Motor_Turn_Left(uint8_t Direction);
void Motor_Turn_Right(uint8_t Direction);

#endif
