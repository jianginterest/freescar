#ifndef _STEERING_ENGINE_H_
#define _STEERING_ENGINE_H_

void steering_engine(void);

void Steering_Engine_Execute(uint32_t duty);
void Steering_Engine_Control(int16 Error);

struct PID{
uint32 Middle;
uint32 Duty_Max;
uint32 Duty_Min;
uint32 P;
uint32 I;
uint32 D;
int16   Current_Error;//ÏÖÔÚµÄÆ«²î
int16   Last_Error;
int16   Last_Last_Error;
uint32 Output;


};

extern  struct PID  Steering_Engine;
#endif