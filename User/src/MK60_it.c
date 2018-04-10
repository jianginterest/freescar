/**
  ******************************************************************************
  * @file       MK60_it.c
  * @author  	Qin xingpeng
  * @version 	V1.0
  * @date     	2017��11����14:39:46
  * @brief     	���������ж�ִ�к���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "include.h"
#include "common.h"
#include "MK60_it.h"



/**
 * @brief   ����0���ж���Ӧִ�к���
 * @param  	None
 * @retval	None
 */
void uart0_Intertupt(void){

}


/**
 * @brief   ����4���ж���Ӧִ�к���
 * @param  	None
 * @retval	None
 */
void uart4_Intertupt(void){

  char ch;
  if(uart_query(UART4) == 1){
    uart_getchar  (UART4, &ch);
  }
  switch(ch){
    case 'P':
      Steering_Engine.P+=1;
      printf("P:%d\r",Steering_Engine.P);
      break;

    case 'D':
       Steering_Engine.D+=20;
       printf("D:%d\r",Steering_Engine.D);
      break;

    case 'I':
       Steering_Engine.I+=1;
       printf("I:%d\r",Steering_Engine.I);
      break;

  }



}


/**
 * @brief   ����5���ж���Ӧִ�к���
 * @param  	None
 * @retval	None
 */
void uart5_Intertupt(void)
{
}

/**
 * @brief     ��ʱ��0�жϷ�����(��������)
 * @param  	None
 * @retval	None
 */
int16 Num_Pluse;
int16 distance;
void PIT0_IRQHandler(void)
{
    Num_Pluse = ftm_quad_get(FTM2);          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM2);
    if(img.rotary!=0)
    {
     distance+=abs(Num_Pluse);
    }
    Motor_PID();
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}

/******************  ����ͷ�ɼ�ͼ��ʹ�õ�����ж�  ******************/

/*!
 *  @brief      PORTA�жϷ����� ������ͷ+lcd�ã�
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0�жϷ����� ������ͷ+lcd�ã�
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/******************  ����ͷ�ɼ�ͼ��ʹ�õ�����жϽ���  ******************/

/**
 * @brief       LPTMR ��ʱ���ж�
 * @param  	None
 * @retval	None
 */
void LPTMR_IRQHandler ()
{
    LED_turn(LED0);
    LPTMR_Flag_Clear();       //���жϱ�־λ
}

/**
 * @brief      2.4G��IRQ�����ж�
 * @param  	None
 * @retval	None
 */

void PORTE_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //���жϱ�־λ

    n = 5;
    if(flag & (1 << n))                                 //PTE5�����ж�
    {
        nrf_handler();
    }
}


/******************* (C) COPYRIGHT 2017 Home of Innovation *****END OF FILE****/