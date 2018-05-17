/**
  ******************************************************************************
  * @file       MK60_it.c
  * @author  	Qin xingpeng
  * @version 	V1.0
  * @date     	2017年11月日14:39:46
  * @brief     	配置所有中断执行函数
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "include.h"
#include "common.h"
#include "MK60_it.h"



/**
 * @brief   串口0的中断响应执行函数
 * @param  	None
 * @retval	None
 */
void uart0_Intertupt(void){

}


/**
 * @brief   串口4的中断响应执行函数
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
 * @brief   串口5的中断响应执行函数
 * @param  	None
 * @retval	None
 */
void uart5_Intertupt(void)
{
}

/**
 * @brief     定时器0中断服务函数(正交解码)
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

    PIT_Flag_Clear(PIT0);       //清中断标志位
}

int32 Num_Pluse1;
void PORTB_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTB_ISFR;
    PORTB_ISFR  = ~0;

     n = 8;
    if(flag & (1 << n))                                 //PTB8触发中断
    {
       Num_Pluse1++;
    }

}
/******************  摄像头采集图像使用的相关中断  ******************/

/*!
 *  @brief      PORTA中断服务函数 （摄像头+lcd用）
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0中断服务函数 （摄像头+lcd用）
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/******************  摄像头采集图像使用的相关中断结束  ******************/

/**
 * @brief       LPTMR 定时器中断
 * @param  	None
 * @retval	None
 */
void LPTMR_IRQHandler ()
{
    LED_turn(LED0);
    LPTMR_Flag_Clear();       //清中断标志位
}

/**
 * @brief      2.4G的IRQ引脚中断
 * @param  	None
 * @retval	None
 */

void PORTE_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //清中断标志位

    n = 5;
    if(flag & (1 << n))                                 //PTE5触发中断
    {
        nrf_handler();
    }
   /*
    n = 24;
    if(flag & (1 << n))                                 //PTE5触发中断
    {
       Num_Pluse1++;
    }
     */

}


/******************* (C) COPYRIGHT 2017 Home of Innovation *****END OF FILE****/