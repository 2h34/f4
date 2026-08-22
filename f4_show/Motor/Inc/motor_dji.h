#ifndef MOTOR_DJI_H
#define MOTOR_DJI_H

#include "motor.h"
#include "dji_motor.h"


float Motor_DJI_GetPosition(Motor_t* motor);
float Motor_DJI_GetSpeed(Motor_t* motor);
bool Motor_DJI_SetPosition(Motor_t* motor, float position);
bool Motor_DJI_SetSpeed(Motor_t* motor, float speed);
void Motor_DJI_Disable(Motor_t* motor);  


bool Motor_DJI_Bind(Motor_t *motor, uint8_t id);

#endif // MOTOR_DJI_H