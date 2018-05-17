#include <include.h>


#define MaxDuty 800

/**
 * @brief   ���������뾶����Ϣ
 *          ���ù�ʽ y=k/x   x=Radius_Bend_Coefficient;  �뾶�����̶ȣ�����
 * @param  	None
 * @retval	None
 */
/*
uint16 Radius_Bend_Coefficient=830;  // kֵ ������ٰ뾶����С�������뾶 y=k/x
uint8  Radius_1000=0;
uint8  Radius_100=0;
uint8  Radius_10=0;
uint8  Radius_1=0;
 */

float speed0=0;
float speed1=0;
int Pulse0;
int Pulse1;

float v_s0=0;
float v_s1=0;

float e_k,e1_k;
int motor0=0;
int motor1=0;
float spavr[1000];
uint32 spcount=0;
float spvv=0 ;
//int32 yuanhuan_flag[4]={0,0,1,center};
uint32 timevar1=0;




#define SPEED_P 40;
float SPeed_set=20;

 /**
 * @brief     ���PID
 * @param  	None
 * @retval	None
 */
struct Motor_PID
/***********   P        I        D         ***********************************/
 Motor_Left ={40,      0,     0,   	         0,  10,0,0,0,0,0},  //5����˼Ϊÿ��5ת
 Motor_Right={60,      7,     80,   	         0,  10,0,0,0,0,0};


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
 *       0.08��ȡÿ����ٰ�ת
 */
/*
 void Motor_PID()
 {
 //  Motor_Differential();
    Motor_Left.SPEED=Motor_Left. Set_Speed;
     Motor_Right.SPEED= Motor_Right. Set_Speed;
 Motor_Left. Pulse_Count =  Num_Pluse*(-0.08);

 Motor_Left. Current_Error = Motor_Left.SPEED - Motor_Left. Pulse_Count;
 Motor_Left.Output+= Motor_Left. P * (Motor_Left. Current_Error- Motor_Left. Last_Error) +
	 Motor_Left. I * (Motor_Left. Current_Error  ) +
	 0.1*Motor_Left. D * (Motor_Left. Current_Error  - 2 * Motor_Left. Last_Error  + Motor_Left. Last_Last_Error);

        Motor_Left. Last_Last_Error = Motor_Left .Last_Error;
	Motor_Left .Last_Error = Motor_Left. Current_Error;

        //---------------

      Motor_Right. Pulse_Count =  Num_Pluse2*(0.08);
      Motor_Right. Current_Error = Motor_Right.SPEED - Motor_Right. Pulse_Count;
      Motor_Right.Output+= Motor_Right. P * (Motor_Right. Current_Error- Motor_Right. Last_Error) +
	 Motor_Right. I * (Motor_Right. Current_Error  ) +
	 0.1*Motor_Left. D * (Motor_Right. Current_Error  - 2 * Motor_Right. Last_Error  + Motor_Right. Last_Last_Error);

        Motor_Right. Last_Last_Error = Motor_Right .Last_Error;
	Motor_Right .Last_Error = Motor_Right. Current_Error;

	//motor_duty(Motor_Left.Output,Motor_Right.Output);
        motor_duty(Motor_Left.Output,0);


 }            */
 /**
 * @brief      ��� ���ֲ��ٺ���
 * @param      differ�з�Χ����3650*��1.8~2.5��/10��
 *           ��1.8~2.5������˼��ת���ʱ�����к����������־���֮�ȵķ�Χ
 * @retval     angleΪƫ�������������ֵ��ƫ�
�� ��ƫ

���������PB8 PB9��DIR��   �ұ�������PA10,PA11
 */
void GetMotorPulse(void){
  Pulse1 = ftm_quad_get(FTM2);          //��ȡFTM �������� ��������(������ʾ������)
  ftm_quad_clean(FTM2);
  Pulse0 =  Num_Pluse1;                                 //���벶׽������������
  Num_Pluse1=0;

  if(img.rotary!=0)
  {
   distance+=abs(Pulse1);
  }
  //��������
  //speed0=(uint8)(Pulse0*0.16325);
//0.09765625 �ٶȲ���ֵ����λ������/��(2017��c��ģ����)  0.008162913602941176470588235��������20ms 0.1632582720588235294117647��������1ms  //�����ۼ�  �ٶ�=��λ�������/ʱ��=������/ʱ��*((28/68)/512)*0.203��̥�ܳ�203mm��
  /*   20msһ�Σ� 1/512*��28/68��*203mm=0.1632582720433  ��mm/20ms��
       *50=8.162913602165��mm/s��  /1000 m/s
  */
  speed0=(Pulse0*0.08162913);

  //Pulse1 =Num_Pluse;
  //Num_Pluse=0;
  //speed1=(uint8)(Pulse1*0.16325);
    speed1=(Pulse1*0.08162913);
 /*
  spavr[spcount++]=speed1;
  if(spcount==1000) spcount=0;
  spvv=0;
  for(int i=0;i<1000;i++)
    spvv+=spavr[i];
  timevar1=(uint32)(spvv/10);
  */

}
uint16  differ=240;
void motor_control(void)
{
  float v_p1,v_p0;
 // float angle=(float) (Steering_Engine.Output-Steering_Engine.Middle);
   if(angle<0)
    {
      v_s1=SPeed_set;
      v_s0=SPeed_set*(1+angle/(differ*10));
        if(v_s0<0)
        {
         v_s0=0;
        }
    }
    else
    {
      v_s1=(SPeed_set*(1-angle/(differ*10)));
      v_s0=SPeed_set;
      if(v_s1<0)
      {  v_s1=0;
      }
    }
    //---------���FTM���---------//
    e_k=v_s0-speed0;
    e1_k=v_s1-speed1;
    v_p0=e_k*SPEED_P;
    v_p1=e1_k*SPEED_P;

    motor0=(int32)(9.330287*v_s0+47.25058+v_p0);
    motor1=(int32)(8.67052*v_s1+60.10405+v_p1);

    motor_duty(motor0,motor1);

}

/**
 * @brief   ��������
 * @param  	None
 * @retval	None
 */
/*

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
             */
