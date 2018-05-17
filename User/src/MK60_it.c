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
int32 Num_Pluse,Num_Pluse2;
int16 distance;
uint8 M=0;
void PIT0_IRQHandler(void)
{



    //Steering_Engine_Control(img.Error);

     if(M)
    {
   // GetMotorPulse();
   // motor_control();
      M=0;
    }
     else M++;

    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}

int32 Num_Pluse1;
void PORTB_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTB_ISFR;
    PORTB_ISFR  = ~0;

     n = 8;
    if(flag & (1 << n))                                 //PTB8�����ж�
    {
       Num_Pluse1++;
    }

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
   /*
    n = 24;
    if(flag & (1 << n))                                 //PTE5�����ж�
    {
       Num_Pluse1++;
    }
     */

}


/******************* (C) COPYRIGHT 2017 Home of Innovation *****END OF FILE****/