#include <include.h>

/***************************                  P   I   D        ************************/
struct PID Steering_Engine={9100,10500,7800,  45,  0, 50,0,0,0,0}; //7500    8600   6200


/**
 * @brief    舵机打角初始化         10800 9900 11900
 *           已设置FTM1模块为占空比基数100000 在100hz下 15300为就舵机中值
16   --86 8
15----84 7
 *           50hz 情况下  中值 0   最大值 0   最小值
 *           80hz情况下
 * @param    None
 * @retval   None
 */
void steering_engine(void)
{
ftm_pwm_init(FTM1, FTM_CH1,80,Steering_Engine.Middle);  //100为频率
}
/**
 * @brief    控制舵机打角最大最小值 ，保护电机
 * @param    None
 * @retval   None
 */
void Steering_Engine_Execute(uint32_t duty)
{
  if(duty>Steering_Engine.Duty_Max)
  { duty=Steering_Engine.Duty_Max ; }
  else if (duty<Steering_Engine.Duty_Min)
  {   duty=Steering_Engine.Duty_Min;
  }


    ftm_pwm_duty(FTM1,FTM_CH1,duty);

}


  /**
 * @brief   对赛道误差进行PID运算，来控制舵机
 *          结构体变量PID已声明为全局变量
 * @param  	Error  赛道中线与当前图像中线的误差
 * @retval	None
 */
float angle=0;
void Steering_Engine_Control(int16 Error){

    Steering_Engine.Current_Error=Error;

    Steering_Engine.Output=Steering_Engine.P * (Steering_Engine.Current_Error) +
                           Steering_Engine.D * (Steering_Engine.Current_Error  - Steering_Engine.Last_Error) +
                           Steering_Engine.I * (Steering_Engine.Current_Error  + Steering_Engine.Last_Error  + Steering_Engine.Last_Last_Error);

    angle=Steering_Engine.Output;
    Steering_Engine.Last_Last_Error= Steering_Engine.Last_Error;
    Steering_Engine.Last_Error     = Steering_Engine.Current_Error;

    Steering_Engine.Output=Steering_Engine.Middle-Steering_Engine.Output;

    Steering_Engine_Execute(Steering_Engine.Output);
}


