#include <include.h>

/**
 * @brief   LED��ʼ��������͵�ƽ��LED������LED0��LED1�ڿ⺯�������ã�
 * @param  	None
 * @retval	None
 */
void led_test(void)
{  LED_init(LED0);
   LED_init(LED1);
   gpio_init(PTC14,GPO,0);    //��ɫ 0
   gpio_init(PTC15,GPO,0);       //��ɫ0
}