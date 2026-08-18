#ifndef PID_H
#define PID_H

typedef struct
{
    double Kp;
    double Ki;
    double Kd;

    double last_error;
    double integral;

    double output_limits;
    double integral_limits;
    int isfirst_feedback; 
} PID_t;

void PID_Init(PID_t *pid,double Kp,double Ki,double Kd,double output_limits,double integral_limits);
double PID_Caculate(PID_t *pid,double target,double current,double Ts);
void PID_Reset(PID_t *pid);

#endif // PID_H