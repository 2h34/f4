#ifndef DJI_MOTOR_H
#define DJI_MOTOR_H

#include "main.h"
#include "pid.h"
#include "can.h"

typedef enum
{
    DJ_Disable = 0,
    DJ_RPM = 1,
    DJ_Position = 2,
    DJ_Current = 3,
    DJ_Zero = 4,
} DJ_motor_mode_t;

typedef struct
{
    uint16_t id;
    int16_t rpm;
    int16_t target_rpm;

    int16_t current;
    int16_t current_cmd;

    double position;
    double target_position;
    int16_t zero_position;

    int16_t encoder;
    int16_t last_encoder;
    int16_t encoder_delta;
    int32_t encoder_total;
    uint8_t encoder_initialized;

    uint16_t pulse_per_round;   
    float reduction_ratio;     

    DJ_motor_mode_t mode;
    PID_t speed_pid;
    PID_t position_pid;

} DJI_motor_t;

void DJI_motor_init(void);
void DJI_motor_Receive(CAN_RxHeaderTypeDef *rx_header,uint8_t *rx_data);
void DJI_motor_Func(void);
void DJI_motor_SetMode(DJI_motor_t *motor,DJ_motor_mode_t mode);
void DJI_motor_AngleCalculate(DJI_motor_t *motor);
void DJI_motor_CurrentTransmit(void);
void DJmotor_SpeedMode(DJI_motor_t *motor);
void DJmotor_CurrentMode(DJI_motor_t *motor);
void DJmotor_PositionMode(DJI_motor_t *motor);

void DJI_motor_Set_Speed(DJI_motor_t *motor,int16_t target_rpm);



#endif // DJI_MOTOR_H