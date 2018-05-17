#ifndef _FTM_CONFIG_H_
#define _FTM_CONFIG_H_

void motor_init(void);
void motor_duty(int32_t MotorL,int32_t MotorR);
void Control_Real_Wheel(void);
void Motor_PID(void);
void Motor_Differential(void);
void GetMotorPulse(void);
void motor_control(void);

struct Motor_PID{
	uint16_t P;
	uint16_t I;
	uint16_t D;
        float SPEED;
        float Set_Speed;
	float    Bend_Coefficient;//ÍäÇúÏµÊý

	float  Current_Error;
	float  Last_Error;
	float  Last_Last_Error;
	int32_t  Output;
	float Pulse_Count;

};

extern struct Motor_PID Motor_Left,Motor_Right;
#endif