#include <include.h>

/**
 * @brief      2.4G�Լ켰����
 * @param  	None
 * @retval	None
 */
void nrf_config(void)
{
   while(!nrf_init())        //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ
   {
       printf("\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\n");
       led_test();
   }
   set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);   //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn ���������ò�����nrf��
   enable_irq(PORTE_IRQn);
   printf("\n      NRF��MCU���ӳɹ���\n");
   nrf_tx_buff[0]=30;//���͵����ݳ���
}
/**
 * @brief      2.4G���ڶ����ֵ    nrf_rx(nrf_rx_buff,10);
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
 * @brief      2.4G���ղ���    nrf_rx(nrf_rx_buff,10);
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
 * @brief      2.4G�����PID    nrf_rx(nrf_rx_buff,10);
 * @param  	None
 * @retval	None
 */
 void nrf_Change_Motorpid(void)
{

   Motor_Left. P=nrf_rx_buff[1];
    Motor_Left. I=nrf_rx_buff[2];
     Motor_Left. D=nrf_rx_buff[3];



}

