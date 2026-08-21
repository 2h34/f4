#include "Lift.h"
#include "ZDrive.h"
#include "math.h"

#define LIFT_MOTOR_ID        1U

/* 暂时假设：
 * 输出轴每转一圈，Lift 移动 20 mm
 */
#define LIFT_MM_PER_REV      20.0f
/* 暂时假设到位误差 ±2 mm */
#define LIFT_TOLERANCE_MM    2.0f

#define LIFT_REACHED_COUNT_THRESHOLD 20U // 连续20次到位才算真正到位
#define LIFT_HOMING_COUNT_THRESHOLD 500U // 连续500次未归零才算故障

#define LIFT_MIN_HEIGHT_MM   0.0f
#define LIFT_MAX_HEIGHT_MM   1000.0f

static Lift_t lift;

/* Lift 高度 mm → 电机角度 deg */
static float Lift_HeightToAngle(float height_mm)
{
    return height_mm / LIFT_MM_PER_REV * 360.0f;
}


/* 电机角度 deg → Lift 高度 mm */
static float Lift_AngleToHeight(float angle_deg)
{
    return angle_deg / 360.0f * LIFT_MM_PER_REV;
}

void Lift_Init(void)
{
    lift.reached_count = 0;// 初始化到位计数器
    lift.homing_count = 0;// 初始化归零计数器
    lift.target_height_mm = 0.0f;
    lift.actual_height_mm = 0.0f;
    lift.tolerance_mm = LIFT_TOLERANCE_MM; // 设置默认的到位允许误差为2mm
    lift.state = LIFT_UNZEROED; // 设置默认状态为未归零
    lift.zero_angle_deg = 0.0f; // 设置默认的归零角度为0度
    Zdrive_Begin(LIFT_MOTOR_ID); // 初始化电机
}

void Lift_SetHeight(float height_mm)
{
    if (lift.state != LIFT_READY && lift.state != LIFT_REACHED)
    {
        return;
    }
    if (height_mm < LIFT_MIN_HEIGHT_MM ||
        height_mm > LIFT_MAX_HEIGHT_MM)
    {
        return;
    }
    lift.reached_count = 0;
    lift.target_height_mm = height_mm;
    float angle_deg_delta = Lift_HeightToAngle(height_mm);
    Zdrive_Set_target_mode(LIFT_MOTOR_ID, Zdrive_Postion, angle_deg_delta+lift.zero_angle_deg);
    lift.state = LIFT_MOVING;
}

void Lift_Update(void)
{
    // 更新当前高度
    lift.actual_height_mm =Lift_AngleToHeight(Zdrive_Get_Position(LIFT_MOTOR_ID) - lift.zero_angle_deg);
}

bool Lift_IsReached(void)
{
    // 检查当前高度是否在目标高度的允许误差范围内
    if (fabsf(lift.actual_height_mm - lift.target_height_mm)
        <= lift.tolerance_mm)
    {
        if (lift.reached_count < LIFT_REACHED_COUNT_THRESHOLD)
        {
            lift.reached_count++;
        }
    }
    else
    {
        lift.reached_count = 0;
    }
    if (lift.reached_count >= LIFT_REACHED_COUNT_THRESHOLD) // 连续20次到位才算真正到位
    {
        return true;
    }
    return false;
}

void Lift_Process(void)
{
    Lift_Update();

    switch (lift.state)
    {
        case LIFT_UNZEROED:
            /* 暂时等待 Zero 命令 */
            break;

        case LIFT_HOMING:
            /* 下一步再实现 */
            if (Lift_HaveZeroed())
            {
                float current_angle = Zdrive_Get_Position(LIFT_MOTOR_ID);
                lift.zero_angle_deg = current_angle;
                Zdrive_Set_target_mode(LIFT_MOTOR_ID, Zdrive_Postion, current_angle); // 停止电机
                lift.target_height_mm = 0.0f;
                lift.actual_height_mm = 0.0f;
                lift.homing_count = 0;
                lift.state = LIFT_READY;
            }
            else
            {
                lift.homing_count++;
                if (lift.homing_count > LIFT_HOMING_COUNT_THRESHOLD) // 超过一定次数仍未归零，进入故障状态
                {
                    lift.state = LIFT_FAULT;
                }
            }
            break;

        case LIFT_READY:
            break;

        case LIFT_MOVING:
            if (Lift_IsReached())
            {
                lift.state = LIFT_REACHED;
            }
            break;

        case LIFT_REACHED:
            break;

        case LIFT_FAULT:
            /* 处理故障状态 */
            break;

        default:
            break;
    }
}

void Lift_Zero(void)
{
    if (lift.state != LIFT_UNZEROED)
    {
        return;
    }
    lift.state = LIFT_HOMING;
    lift.homing_count = 0;
    Zdrive_Set_target_mode(LIFT_MOTOR_ID,Zdrive_Speed,-50.0f); // 以 -50 rpm 的速度向下移动
}

bool Lift_HaveZeroed(void)
{
    return Zdrive_Get_Position(LIFT_MOTOR_ID) <= 0.0f; // 假设归零位置为电机角度 0 度
}