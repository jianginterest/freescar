#include <include.h>

/**
 * @brief   ���ڳ�ʼ��,����ѡ��Ĵ��жϺ���
 * @param  	None
 * @retval	None
 */

void usart_init(void)
{
     uart_init(UART4,115200);
     set_vector_handler(UART4_RX_TX_VECTORn,uart4_Intertupt);
     //�����ж�����������жϷ����� (uart4_Intertupt)
     // UART4_RX_TX_VECTORn:UART0 Receive/Transmit interrupt
     uart_rx_irq_en (UART4);//ʹ�ܽ����ж�
}

