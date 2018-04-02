#include <include.h>

/**
 * @brief      2.4G自检及配置
 * @param  	None
 * @retval	None
 */
void nrf_config(void)
{
   while(!nrf_init())        //初始化NRF24L01+ ,等待初始化成功为止
   {
       printf("\n  NRF与MCU连接失败，请重新检查接线。\n");
       led_test();
   }
   set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);   //设置 PORTE 的中断服务函数为 PORTE_VECTORn （必须设置才能用nrf）
   enable_irq(PORTE_IRQn);
   printf("\n      NRF与MCU连接成功！\n");
   nrf_tx_buff[0]=30;//发送的数据长度
}
/**
 * @brief      2.4G调节舵机中值    nrf_rx(nrf_rx_buff,10);
 * @param  	None
 * @retval	None
 */
void nrf_measure_Angle(void)
{
 if(nrf_rx_buff[1]=='a')
 {
 Steering_Engine.Middle+=50;
 ftm_pwm_duty(FTM1,FTM_CH1, Steering_Engine.Middle);
 nrf_rx_buff[1]=0;
 }
  if(nrf_rx_buff[1]=='b')
  {
 Steering_Engine.Middle-=50;
 ftm_pwm_duty(FTM1,FTM_CH1, Steering_Engine.Middle);
  nrf_rx_buff[1]=0;
  }
}
 /**
 * @brief      2.4G接收测试    nrf_rx(nrf_rx_buff,10);
 * @param  	None
 * @retval	None
 */

void nrf_test(void)
{
  if(nrf_rx_buff[1]=='a')
 {
 led_test();
 nrf_rx_buff[1]=0;
 }

}

/**
 * @brief      2.4G调电机PID    nrf_rx(nrf_rx_buff,10);
 * @param  	None
 * @retval	None
 */
 void nrf_Change_Motorpid(void)
{

   Motor_Left. P=nrf_rx_buff[1];
    Motor_Left. I=nrf_rx_buff[2];
     Motor_Left. D=nrf_rx_buff[3];



}

