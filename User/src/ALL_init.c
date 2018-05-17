#include <include.h>

 /**
 * @brief      所有函数初始化
 * @param  	None
 * @retval	None
 */
void all_init(void)
{
   NVIC_cnfig();
   //usart_init();                //设置串口中断服务函数
   LCD_flash_read();
   LED_init(LED0);            //初始化LED0，LPTMR中断用到LED0
  //  led_test();
   //port_init (PTE24, IRQ_RISING | PF | ALT1 | PULLUP );  //PTE24 上升沿触发中断


   port_init (PTB8, IRQ_RISING | PF | ALT1 | PULLUP );
   set_vector_handler(PORTB_VECTORn ,PORTB_IRQHandler);   //设置 PORTE 的中断服务函数为 PORTE_VECTORn （必须设置才能用nrf）
   enable_irq(PORTB_IRQn);


  // nrf_config();             //2.4G配置函数
   key_init(KEY_MAX);       //初始化所有按键
   LCD_camera_init();      //摄像头+LCD初始化
   FTM_Quard();           //正交解码
   motor_init();
   steering_engine();


}