#include <include.h>

 /**
 * @brief      ���к�����ʼ��
 * @param  	None
 * @retval	None
 */
void all_init(void)
{
   NVIC_cnfig();
   //usart_init();                //���ô����жϷ�����
   LCD_flash_read();
   LED_init(LED0);            //��ʼ��LED0��LPTMR�ж��õ�LED0
  //  led_test();
   //port_init (PTE24, IRQ_RISING | PF | ALT1 | PULLUP );  //PTE24 �����ش����ж�


   port_init (PTB8, IRQ_RISING | PF | ALT1 | PULLUP );
   set_vector_handler(PORTB_VECTORn ,PORTB_IRQHandler);   //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn ���������ò�����nrf��
   enable_irq(PORTB_IRQn);


  // nrf_config();             //2.4G���ú���
   key_init(KEY_MAX);       //��ʼ�����а���
   LCD_camera_init();      //����ͷ+LCD��ʼ��
   FTM_Quard();           //��������
   motor_init();
   steering_engine();


}