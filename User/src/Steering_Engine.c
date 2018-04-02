#include <include.h>

/********************************************** P   I   D*****/
struct PID  Steering_Engine={13500,15300,11500, 84, 0, 5,  0};

/**
 * @brief    �����ǳ�ʼ��
 *           ������FTM1ģ��Ϊռ�ձȻ���100000 ��100hz�� 15300Ϊ�Ͷ����ֵ
 *           50hz �����  ��ֵ 0   ���ֵ 0   ��Сֵ
 *           100hz�����
 * @param    None
 * @retval   None
 */
void steering_engine(void)
{
ftm_pwm_init(FTM1, FTM_CH1,100,Steering_Engine.Middle);  //100ΪƵ��
}
/**
 * @brief    ���ƶ����������Сֵ ���������
 * @param    None
 * @retval   None
 */
void Steering_Engine_Execute(uint32_t duty)
{
  if(duty>Steering_Engine.Duty_Max)
  { duty=Steering_Engine.Duty_Max ; }
  else if (duty<Steering_Engine.Duty_Min)
  {   duty=Steering_Engine.Duty_Min;
  }else
  {  ftm_pwm_duty(FTM1,FTM_CH1,duty);
  }
}


  /**
 * @brief   ������������PID���㣬�����ƶ��
 *          �ṹ�����PID������Ϊȫ�ֱ���
 * @param  	Error  ���������뵱ǰͼ�����ߵ����
 * @retval	None
 */
void Steering_Engine_Control(int8 Error){
    Steering_Engine.Current_Error=Error;

    Steering_Engine.Output=Steering_Engine.P * (Steering_Engine.Current_Error) +
                           Steering_Engine.D * (Steering_Engine.Current_Error  - Steering_Engine.Last_Error) +
                           Steering_Engine.I * (Steering_Engine.Current_Error  + Steering_Engine.Last_Error  + Steering_Engine.Last_Last_Error);

    Steering_Engine.Last_Last_Error= Steering_Engine.Last_Error;
    Steering_Engine.Last_Error     = Steering_Engine.Current_Error;

    Steering_Engine.Output=Steering_Engine.Middle-Steering_Engine.Output;

    Steering_Engine_Execute(Steering_Engine.Output);
}


