#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/**
 * @brief   �ײ�⺯��ͷ�ļ�
 * @param  	None
 * @retval	None
 */
#include  "MK60_it.h"
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_MMA7455.h"      //������ٶ�MMA7455
#include  "VCAN_NRF24L0.h"      //����ģ��NRF24L01+
#include  "VCAN_RTC_count.h"    //RTC ʱ��ת��
#include  "VCAN_camera.h"       //����ͷ��ͷ�ļ�
#include  "VCAN_LCD.h"          //Һ����ͷ�ļ�

//#include  "ff.h"                //FatFs
#include  "VCAN_computer.h"     //�๦�ܵ�������
#include <math.h>
/**
 * @brief   �û��Զ���ͷ�ļ�
 * @param  	None
 * @retval	None
 */
#include <main.h>
#include <led.h>
#include <uart_config.h>
#include <image_deal.h>
#include <ALL_init.h>
#include <FTM_Quadrature Decoder.h>
#include <pit_config.h>
#include <Steering_Engine.h>
#include <NRF24L.h>
#include <FTM_config.h>
#include <LCD.h>



/**
 * @brief       ȫ�ֱ���
 * @param  	None
 * @retval	None
 */
extern uint8 nrf_tx_buff[32];
extern uint8 nrf_rx_buff[32];
extern struct IMG img;
extern struct least_square_method LSM;


#endif  //__INCLUDE_H__
