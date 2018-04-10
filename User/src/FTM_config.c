#include <include.h>


#define MaxDuty 800

/**
 * @brief   ���������뾶����Ϣ
 *          ���ù�ʽ y=k/x   x=Radius_Bend_Coefficient;  �뾶�����̶ȣ�����
 * @param  	None
 * @retval	None
 */
uint16 Radius_Bend_Coefficient=830;  // kֵ ������ٰ뾶����С�������뾶 y=k/x
uint8  Radius_1000=0;
uint8  Radius_100=0;
uint8  Radius_10=0;
uint8  Radius_1=0;

 /**
 * @brief     ���PID
 * @param  	None
 * @retval	None
 */
struct Motor_PID
/***********   P        I        D         ***********************************/
 Motor_Left ={60,      7,     80,   	         0,  8,0,0,0,0,0},  //5����˼Ϊÿ��5ת
 Motor_Right={60,      7,     80,   	         0,  8,0,0,0,0,0};


 /**
 * @brief      �����ʼ��
 * @param  	20000ΪƵ��
 * @retval	None
 */
void motor_init(void)
{
    ftm_pwm_init(FTM0, FTM_CH4,20000,0);
    ftm_pwm_init(FTM0, FTM_CH5,20000,0);
    ftm_pwm_init(FTM0, FTM_CH6,20000,0);
    ftm_pwm_init(FTM0, FTM_CH7,20000,0);
}

 /**
 * @brief      ռ�ձ��������
 * @param  	FTM0 ����Ϊ1000
 * @retval	None
 */
void motor_duty(int32_t MotorL,int32_t MotorR)
{
  if(MotorL>MaxDuty) MotorL=MaxDuty;
  else if(MotorL<-MaxDuty) MotorL=-MaxDuty;
  if(MotorR>MaxDuty) MotorR=MaxDuty;
  else if(MotorR<-MaxDuty) MotorR=-MaxDuty;

  if(MotorL>0)
  {ftm_pwm_duty(FTM0, FTM_CH6, MotorL);
  ftm_pwm_duty(FTM0, FTM_CH7,0 );
  }
  else
    {ftm_pwm_duty(FTM0, FTM_CH6, 0);
  ftm_pwm_duty(FTM0, FTM_CH7, -MotorL);
  }
  if(MotorR>0)
  { ftm_pwm_duty(FTM0, FTM_CH5, 0);
  ftm_pwm_duty(FTM0, FTM_CH4, MotorR);

  }
   else
   {
    ftm_pwm_duty(FTM0, FTM_CH5,  -MotorR);
    ftm_pwm_duty(FTM0, FTM_CH4,0);

   }

}
/**
 * @brief      ���PID����
 * @param
 * @retval	0.203��̥�ܳ���203mm��
 *       ������ת��Ȧ����������/512 ��ת��Ȧ��*���������ֵĳ�����28��=�ܵĳ�������
 *      �ܵĳ�����/�����������68�� =�����ߵ�Ȧ������Ϊ��������ʱ�ж�20msһ�Σ�����Ȧ��/ʱ��=�ٶ�
 *      Num_Pluse��1��/512*28/68/20ms*1000��ת�����룩= 0.0402113970588
 *       0.02��ȡÿ����ٰ�ת
 */
 void Motor_PID()
 {
   Motor_Differential();
 Motor_Left. Pulse_Count =  Num_Pluse*(-0.08);

 Motor_Left. Current_Error = Motor_Left.SPEED - Motor_Left. Pulse_Count;
 Motor_Left.Output+= Motor_Left. P * (Motor_Left. Current_Error- Motor_Left. Last_Error) +
	 Motor_Left. I * (Motor_Left. Current_Error  ) +
	 0.1*Motor_Left. D * (Motor_Left. Current_Error  - 2 * Motor_Left. Last_Error  + Motor_Left. Last_Last_Error);

        Motor_Left. Last_Last_Error = Motor_Left .Last_Error;
	Motor_Left .Last_Error = Motor_Left. Current_Error;

        //---------------
      Motor_Right. Pulse_Count =  Num_Pluse*(-0.08);
      Motor_Right. Current_Error = Motor_Right.SPEED - Motor_Right. Pulse_Count;
      Motor_Right.Output+= Motor_Right. P * (Motor_Right. Current_Error- Motor_Right. Last_Error) +
	 Motor_Right. I * (Motor_Right. Current_Error  ) +
	 0.1*Motor_Left. D * (Motor_Right. Current_Error  - 2 * Motor_Right. Last_Error  + Motor_Right. Last_Last_Error);

        Motor_Right. Last_Last_Error = Motor_Right .Last_Error;
	Motor_Right .Last_Error = Motor_Right. Current_Error;

	motor_duty(Motor_Left.Output,Motor_Right.Output);


 }
 /**
 * @brief      ��� ���ֲ��ٺ���
 * @param      differ�з�Χ����3650*��1.8~2.5��/10��
 *           ��1.8~2.5������˼��ת���ʱ�����к����������־���֮�ȵķ�Χ
 * @retval     angleΪƫ�������������ֵ��ƫ�
�� ��ƫ
 */
uint16  differ=600;
void Motor_Differential(void)
{
 int32 angle=Steering_Engine.Output-Steering_Engine.Middle;

  if(angle<0)
    {
      Motor_Left.SPEED=Motor_Left. Set_Speed;
      Motor_Right.SPEED=Motor_Right. Set_Speed*(1+angle/(differ*10));
    }
    else
    {
      Motor_Left.SPEED=Motor_Left. Set_Speed*(1-angle/(differ*10));
      Motor_Right.SPEED= Motor_Right. Set_Speed;
    }

}

/**
 * @brief   ��������
 * @param  	None
 * @retval	None
 */

void Control_Real_Wheel(void){
    if(img.Error>30){
        img.Error=30;
    }else if(img.Error<-30){
         img.Error=-30;
    }else{
    }

    if(abs(img.Error)<2){
    }

    if(img.Error==0){
        Radius_1000=0;
        Radius_100=0;
        Radius_10=0;
        Radius_1=0;
    }else{
        Radius_1000 = (int)((Radius_Bend_Coefficient/abs(img.Error)-8)/1000);
        Radius_100  = (int)((Radius_Bend_Coefficient/abs(img.Error)-8)/100  -Radius_1000*10);
        Radius_10   = (int)((Radius_Bend_Coefficient/abs(img.Error)-8)/10   -Radius_1000*100-Radius_100*10);
        Radius_1    = (int)((Radius_Bend_Coefficient/abs(img.Error)-8)%10);
    }



    if(img.Error>0){
          //����ת
    }else if(img.Error<0){
          //����ת
    }else if(img.Error==0){

    }else{
    }
}

