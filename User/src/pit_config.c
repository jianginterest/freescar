#include <include.h>

/**
 * @brief       ��ʱ��PIT����
 * @param  	None
 * @retval	None
 */
void pit_config(void)
{
 pit_init_ms(PIT0, 20);                                 //��ʼ��PIT0����ʱʱ��Ϊ�� 20ms
 set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
 enable_irq (PIT0_IRQn);                                 //ʹ��PIT0�ж�
}

/**
 * @brief       �ж����ȼ�
 * @param  	None
 * @retval	None
 */
void  NVIC_cnfig(void)
{
  NVIC_SetPriorityGrouping(5);                                           //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
  NVIC_SetPriority(PORTA_IRQn,0);                                        //�������ȼ�
  NVIC_SetPriority(DMA0_IRQn,1);                                         //�������ȼ�
  NVIC_SetPriority(PORTE_IRQn,2);                                        //�������ȼ�
  NVIC_SetPriority(PIT0_IRQn,3);
  NVIC_SetPriority(UART4_RX_TX_IRQn,3);
}



