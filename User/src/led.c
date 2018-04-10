#include <include.h>

/**
 * @brief   LED初始化，输出低电平（LED的引脚LED0，LED1在库函数里设置）
 * @param  	None
 * @retval	None
 */
void led_test(void)
{  LED_init(LED0);
   LED_init(LED1);
   gpio_init(PTC14,GPO,0);    //蓝色 0
   gpio_init(PTC15,GPO,0);       //绿色0
}