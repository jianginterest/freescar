#include <include.h>

/**
 * @brief       定时器PIT配置
 * @param  	None
 * @retval	None
 */
void pit_config(void)
{
 pit_init_ms(PIT0, 20);                                 //初始化PIT0，定时时间为： 20ms
 set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
 enable_irq (PIT0_IRQn);                                 //使能PIT0中断
}

/**
 * @brief       中断优先级
 * @param  	None
 * @retval	None
 */
void  NVIC_cnfig(void)
{
  NVIC_SetPriorityGrouping(5);                                           //设置优先级分组,4bit 抢占优先级,没有亚优先级
  NVIC_SetPriority(PORTA_IRQn,0);                                        //配置优先级
  NVIC_SetPriority(DMA0_IRQn,1);                                         //配置优先级
  NVIC_SetPriority(PORTE_IRQn,2);                                        //配置优先级
  NVIC_SetPriority(PIT0_IRQn,3);
  NVIC_SetPriority(UART4_RX_TX_IRQn,3);
}



