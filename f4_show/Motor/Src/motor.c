#include "motor.h"
#include "ZDrive.h"
#include "dji_motor.h"


/*建立 Motor 与具体 Driver 实例的绑定，并完成该实例为了接受后续通用 Motor 命令所必须的 backend-specific readiness 操作。*/ 
bool Motor_Init(Motor_t *motor,MotorType_t type,uint8_t id)
{
    if (motor == NULL)
    {
        return false;
    }
    if (type != MOTOR_TYPE_ZDRIVE && type != MOTOR_TYPE_DJI)
    {
        return false;
    }
    motor->type = type;
    motor->id = id;
    switch (type)
    {
        case MOTOR_TYPE_ZDRIVE:
            if (id < 1 || id > USE_ZDRIVE_NUM) /* ZDrive ID 范围检查 */
            {
                return false;
            }
            Zdrive_Begin(id);
            break;

        case MOTOR_TYPE_DJI:
            if (DJI_motor_GetById(id) == NULL)  /* DJI ID 范围检查，看取出来的是不是空指针 */
            {
                return false;
            }
            // 当前无需额外实例启动
            break;
    }
    return true;
}

bool Motor_SetPosition(Motor_t* motor, float position)
{
    if (motor == NULL)
    {
        return false;
    }
    switch (motor->type)
    {
    case MOTOR_TYPE_ZDRIVE:
        // 转成 ZDrive 的位置控制
        Zdrive_Set_target_mode(motor->id,Zdrive_Postion,position);
        break;

    case MOTOR_TYPE_DJI:
        // 转成 DJI 的位置控制
        {
            DJI_motor_t *dji = DJI_motor_GetById(motor->id);
            if (dji == NULL)
            {
                return false;
            }
            DJI_motor_SetMode(dji,DJ_Position);
            DJI_motor_Set_Position(dji,position);
            break;
        }
    }
    return true;
}

bool Motor_SetSpeed(Motor_t* motor, float speed)
{
    if (motor == NULL)
    {
        return false;
    }
    switch (motor->type)
    {
    case MOTOR_TYPE_ZDRIVE:
        // 转成 ZDrive 的速度控制
        {
        Zdrive_Set_target_mode(motor->id,Zdrive_Speed,speed);
        break;
        }
    case MOTOR_TYPE_DJI:
        // 转成 DJI 的速度控制
        {
        DJI_motor_t *dji = DJI_motor_GetById(motor->id);
        if (dji == NULL)
        {
            return false;
        }
        DJI_motor_SetMode(dji,DJ_RPM);
        DJI_motor_Set_Speed(dji,speed);
        break;
        }
    }
    return true;
}

float Motor_GetPosition(Motor_t* motor)
{
    if (motor == NULL)
    {
        return 0.0f;
    }
    switch (motor->type)
    {
    case MOTOR_TYPE_ZDRIVE:
        return Zdrive_Get_Position(motor->id);
    case MOTOR_TYPE_DJI:
        {
        DJI_motor_t *dji = DJI_motor_GetById(motor->id);
        if (dji == NULL)
        {
            return 0.0f;
        }
        return dji->position;
        }
    default:
        return 0.0f;
    }
}

float Motor_GetSpeed(Motor_t* motor)
{
    if (motor == NULL)
    {
        return 0.0f;
    }
    switch (motor->type)
    {
    case MOTOR_TYPE_ZDRIVE:
        return Zdrive_GetSpeed(motor->id);
    case MOTOR_TYPE_DJI:
        {
        DJI_motor_t *dji = DJI_motor_GetById(motor->id);
        if (dji == NULL)
        {
            return 0.0f;
        }
        return dji->rpm;
        }
    default:
        return 0.0f;
    }
}

void Motor_Disable(Motor_t* motor)
{
    if (motor == NULL)
    {
        return;
    }
    switch (motor->type)
    {
    case MOTOR_TYPE_ZDRIVE:
        Zdrive_Set_target_mode(motor->id,Zdrive_Disable,0.0f);
        break;
    case MOTOR_TYPE_DJI:
        {
        DJI_motor_t *dji = DJI_motor_GetById(motor->id);
        if (dji == NULL)
            {
                return;
            }
        DJI_motor_SetMode(dji,DJ_Disable);
        break;
        }
    }
}