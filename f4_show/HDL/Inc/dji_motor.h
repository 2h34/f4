#ifndef DJI_MOTOR_H
#define DJI_MOTOR_H

#include "main.h"
#include "pid.h"
#include "can.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

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

    int16_t current;  //Current是转矩
    int16_t current_cmd;
    int16_t target_current;

    double position;
    double target_position;

    int16_t zero_rpm;
    uint32_t zero_cnt;
    int16_t zero_current_limit;
    int16_t zero_distance;
    uint8_t zero_flag;

    int16_t encoder;
    int16_t last_encoder;
    int16_t encoder_delta;
    int32_t encoder_total;
    uint8_t encoder_initialized;

    uint16_t pulse_per_round;   
    float reduction_ratio;     

    DJ_motor_mode_t mode;
    DJ_motor_mode_t mode_set;
    PID_t speed_pid;
    PID_t position_pid;

    uint8_t feedback_valid;

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
void DJmotor_ZeroMode(DJI_motor_t *motor);

void DJI_motor_Set_Speed(DJI_motor_t *motor,int16_t target_rpm);
void DJI_motor_Set_Position(DJI_motor_t *motor,double target_position);
void DJI_motor_Set_Current(DJI_motor_t *motor, int16_t target_current);
void DJI_motor_Set_Zero(DJI_motor_t *motor);
    
int16_t ClampPeak(int16_t value, int16_t limit);
void DJmotor_SwitchMode(DJI_motor_t *motor);


#endif // DJI_MOTOR_H