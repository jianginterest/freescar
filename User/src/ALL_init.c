#include <include.h>

 /**
 * @brief      所有函数初始化
 * @param  	None
 * @retval	None
 */
void all_init(void)
{
   NVIC_cnfig();
   usart_init();                //设置串口中断服务函数
   LED_init(LED0);            //初始化LED0，LPTMR中断用到LED0
  //  led_test();
   nrf_config();             //2.4G配置函数
   key_init(KEY_MAX);       //初始化所有按键
   LCD_camera_init();      //摄像头+LCD初始化
   FTM_Quard();           //正交解码
   motor_init();
   steering_engine();


}