#ifndef LIFT_H
#define LIFT_H

#include <stdbool.h>

typedef struct
{
    float target_height_mm;   // 目标高度
    float actual_height_mm;   // 当前高度
    float tolerance_mm;       // 到位允许误差
} Lift_t;

typedef enum
{
    LIFT_UNZEROED,
    LIFT_HOMING,
    LIFT_READY,
    LIFT_MOVING,
    LIFT_REACHED,
    LIFT_FAULT
} LiftState;

void Lift_Init(void);
void Lift_SetHeight(float height_mm);
void Lift_Update(void);
bool Lift_Isreached(void);



#endif /* LIFT_H */