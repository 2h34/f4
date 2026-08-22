#include "motor_zdrive.h"


static const MotorOps_t zdrive_ops =
{
    .set_position = Motor_ZDrive_SetPosition,
    .set_speed    = Motor_ZDrive_SetSpeed,
    .get_position = Motor_ZDrive_GetPosition,
    .get_speed    = Motor_ZDrive_GetSpeed,
    .disable      = Motor_ZDrive_Disable,
};


bool Motor_ZDrive_Bind(Motor_t *motor, uint8_t id)
{
    if (id < 1 || id > USE_ZDRIVE_NUM) /* ZDrive ID 范围检查 */
        {
            return false;
        }
    Zdrive_Begin(id);
    motor->ops = &zdrive_ops;
    return true;
}

float Motor_ZDrive_GetPosition(Motor_t* motor)
{
    return Zdrive_Get_Position(motor->id);
}

float Motor_ZDrive_GetSpeed(Motor_t* motor)
{
    return Zdrive_GetSpeed(motor->id);
}

void Motor_ZDrive_Disable(Motor_t* motor)
{
    Zdrive_Set_target_mode(motor->id, Zdrive_Disable, 0.0f);
}

bool Motor_ZDrive_SetSpeed(Motor_t* motor, float speed)
{
    Zdrive_Set_target_mode(motor->id,Zdrive_Speed,speed);
    return true;
}

bool Motor_ZDrive_SetPosition(Motor_t* motor, float position)
{
    Zdrive_Set_target_mode(motor->id,Zdrive_Position,position);
    return true;
}