#include "pid.h"

void PID_Init(PID_t *pid,double Kp,double Ki,double Kd,double output_limits,double integral_limits)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->last_error = 0.0;
    pid->integral = 0.0;

    pid->output_limits = output_limits;
    pid->integral_limits = integral_limits;
}

double PID_Caculate(PID_t *pid,double target,double current,double Ts)
{
    double error = target - current;
    pid->integral += error*Ts;

    if (pid->integral > pid->integral_limits)
    {
        pid->integral = pid->integral_limits;
    }
    else if (pid->integral < -pid->integral_limits)
    {
        pid->integral = -pid->integral_limits;
    }

    double derivative = (error - pid->last_error) / Ts;
    double output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    if (output > pid->output_limits)
    {
        output = pid->output_limits;
    }
    else if (output < -pid->output_limits)
    {
        output = -pid->output_limits;
    }

    pid->last_error = error;

    return output;
}

void PID_Reset(PID_t *pid)
{
    pid->last_error = 0.0;
    pid->integral = 0.0;
}


