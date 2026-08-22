#ifndef MOTOR_ZDRIVE_H
#define MOTOR_ZDRIVE_H


#include "motor.h"
#include "ZDrive.h"


float Motor_ZDrive_GetPosition(Motor_t* motor);
float Motor_ZDrive_GetSpeed(Motor_t* motor);
bool Motor_ZDrive_SetPosition(Motor_t* motor, float position);
bool Motor_ZDrive_SetSpeed(Motor_t* motor, float speed);
void Motor_ZDrive_Disable(Motor_t* motor);  


bool Motor_ZDrive_Bind(Motor_t *motor, uint8_t id);



#endif // MOTOR_ZDRIVE_H