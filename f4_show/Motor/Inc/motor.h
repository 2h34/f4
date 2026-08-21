#ifndef Motor_H
#define Motor_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    MOTOR_TYPE_ZDRIVE,
    MOTOR_TYPE_DJI
} MotorType_t;

typedef struct
{
    MotorType_t type;
    uint8_t id;
} Motor_t;

/*规定：速度：单位为 RPM，为输出轴速度。位置：单位为度*/

float Motor_GetPosition(Motor_t* motor);
float Motor_GetSpeed(Motor_t* motor);
bool Motor_SetPosition(Motor_t* motor, float position);
bool Motor_SetSpeed(Motor_t* motor, float speed);
bool Motor_Init(Motor_t *motor,MotorType_t type,uint8_t id);
void Motor_Disable(Motor_t* motor);  /*取消该 Motor 当前的主动控制，使其不再维持 Position / Speed 目标，并请求底层停止主动驱动输出。*/

#endif // Motor_H
