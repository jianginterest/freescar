#include <include.h>

/**
 * @brief   串口初始化,并有选择的打开中断函数
 * @param  	None
 * @retval	None
 */

void usart_init(void)
{
     uart_init(UART4,115200);
     set_vector_handler(UART4_RX_TX_VECTORn,uart4_Intertupt);
     //设置中断向量表里的中断服务函数 (uart4_Intertupt)
     // UART4_RX_TX_VECTORn:UART0 Receive/Transmit interrupt
     uart_rx_irq_en (UART4);//使能接收中断
}

