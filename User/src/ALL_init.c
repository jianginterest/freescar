#include <include.h>

 /**
 * @brief      ���к�����ʼ��
 * @param  	None
 * @retval	None
 */
void all_init(void)
{
   NVIC_cnfig();
   usart_init();                //���ô����жϷ�����
   LED_init(LED0);            //��ʼ��LED0��LPTMR�ж��õ�LED0
  //  led_test();
   nrf_config();             //2.4G���ú���
   key_init(KEY_MAX);       //��ʼ�����а���
   LCD_camera_init();      //����ͷ+LCD��ʼ��
   FTM_Quard();           //��������
   motor_init();
   steering_engine();


}