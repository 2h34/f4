#ifndef LIFT_H
#define LIFT_H

#include <stdbool.h>
#include <stdint.h>



typedef enum
{
    LIFT_UNZEROED,
    LIFT_HOMING,
    LIFT_READY,
    LIFT_MOVING,
    LIFT_REACHED,
    LIFT_FAULT
} LiftState;


typedef struct
{
    float target_height_mm;   // 目标高度
    float actual_height_mm;   // 当前高度
    float tolerance_mm;       // 到位允许误差
    float zero_angle_deg;   // 归零时的电机角度
    LiftState state;
    uint16_t reached_count;     // 到位计数器
    uint32_t homing_count;      // 归零计数器
} Lift_t;


void Lift_Init(void);
void Lift_SetHeight(float height_mm);
void Lift_Update(void);
bool Lift_IsReached(void);
void Lift_Process(void);
void Lift_Zero(void);
bool Lift_HaveZeroed(void);

#endif /* LIFT_H */