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
    lift.target_height_mm = 0.0f;
    lift.actual_height_mm = 0.0f;
    lift.tolerance_mm = LIFT_TOLERANCE_MM; // 设置默认的到位允许误差为2mm
    Zdrive_Begin(LIFT_MOTOR_ID); // 初始化电机
};

void Lift_SetHeight(float height_mm)
{
    lift.target_height_mm = height_mm;
     float angle_deg = Lift_HeightToAngle(height_mm);
    Zdrive_Set_target_mode(LIFT_MOTOR_ID, Zdrive_Postion, angle_deg);
}

void Lift_Update(void)
{
    // 更新当前高度
    lift.actual_height_mm =Lift_AngleToHeight(Zdrive_Get_position(LIFT_MOTOR_ID));
}

bool Lift_Isreached(void)
{
    // 检查当前高度是否在目标高度的允许误差范围内
    return (fabs(lift.actual_height_mm - lift.target_height_mm) <= lift.tolerance_mm);
}