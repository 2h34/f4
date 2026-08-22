#include "motor_dji.h"

static const MotorOps_t dji_ops =
{
    .set_position = Motor_DJI_SetPosition,
    .set_speed    = Motor_DJI_SetSpeed,
    .get_position = Motor_DJI_GetPosition,
    .get_speed    = Motor_DJI_GetSpeed,
    .disable      = Motor_DJI_Disable,
};

bool Motor_DJI_Bind(Motor_t *motor, uint8_t id)
{
    if (DJI_motor_GetById(id) == NULL) /* DJI ID 范围检查，看取出来的是不是空指针 */
    {
        return false;
    }

    motor->ops = &dji_ops;

    return true;
}

float Motor_DJI_GetPosition(Motor_t* motor)
{
    DJI_motor_t *dji = DJI_motor_GetById(motor->id);
    if (dji == NULL)
    {
        return 0.0f;
    }
    return dji->position;
}

float Motor_DJI_GetSpeed(Motor_t* motor)
{
    DJI_motor_t *dji = DJI_motor_GetById(motor->id);
    if (dji == NULL)
    {
        return 0.0f;
    }
    return dji->rpm;
}

void Motor_DJI_Disable(Motor_t* motor)
{
    DJI_motor_t *dji = DJI_motor_GetById(motor->id);
    if (dji == NULL)
        {
            return;
        }
    DJI_motor_SetMode(dji,DJ_Disable);
}

bool Motor_DJI_SetSpeed(Motor_t* motor, float speed)
{
    DJI_motor_t *dji = DJI_motor_GetById(motor->id);
    if (dji == NULL)
    {
        return false;
    }
    DJI_motor_SetMode(dji,DJ_RPM);
    DJI_motor_Set_Speed(dji,speed);
    return true;
}

bool Motor_DJI_SetPosition(Motor_t* motor, float position)
{
    DJI_motor_t *dji = DJI_motor_GetById(motor->id); //根据id获取对应的电机实例
    if (dji == NULL)
        {
            return false;
        }
    DJI_motor_SetMode(dji,DJ_Position);
    DJI_motor_Set_Position(dji,position);
    return true;
}

