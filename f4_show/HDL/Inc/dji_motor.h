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
    int16_t raw_rpm;  // 电机侧原始 CAN rpm
    float rpm;       // 输出轴转速，单位为 RPM
    float target_rpm;//输出轴目标速度

    int16_t current;  //Current是转矩
    int16_t current_cmd;
    int16_t target_current;

    double position; //输出轴位置
    double target_position; //输出轴目标位置

    float zero_rpm;
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


void DJI_motor_Set_Speed(DJI_motor_t *motor,float target_rpm);
void DJI_motor_Set_Position(DJI_motor_t *motor,double target_position);
void DJI_motor_Set_Current(DJI_motor_t *motor, int16_t target_current);
void DJI_motor_Set_Zero(DJI_motor_t *motor);

DJI_motor_t *DJI_motor_GetById(uint8_t id);

    




#endif // DJI_MOTOR_H