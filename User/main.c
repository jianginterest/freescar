#include <include.h>


uint8 nrf_tx_buff[32]={0};
uint8 nrf_rx_buff[32]={0};

/***************虚拟示波器需要的参数*************/
int di=10000;
uint8_t cmdf[3] = {2,CMD_IMG, ~CMD_IMG};
uint8_t cmdr[3] = {2,~CMD_IMG, CMD_IMG};
/***********************************************/

void main(void)
{
    all_init();
   motor_duty(260,280);
    /*使用虚拟示波器时定义*/
    //temp[0]=0;
    //nrf_tx_buff[0]=1;
    /*****************/
    //for(i=1;i<10;i++)
    //nrf_tx_buff[i]=i;

    while(1)
    {
      camera_get_img(); //摄像头获取图像
      img_extract((uint8*) img.imgbuff,(uint8*)img.img_bin_buff, CAMERA_H*CAMERA_W/8); //60*80
      //vcan_sendimg(img.img_bin_buff, sizeof(img.img_bin_buff));  //发送图像到上位机
     Scan_Img_Array();
     Calc_Track_Error();
     key_image();
     Steering_Engine_Control(img.Error);

     //Tracking();
      // nrf_rx(nrf_rx_buff,5);
        //nrf_measure_Angle();
     // labview(); //虚拟示波器，看PID

    }
}













/**
 * @brief   虚拟示波器函数，放在while（1）中
 * @param   num为编码器采集的数值，在中断中
 * @retval	None
 */
void labview(void)
{
  pit_time_start(PIT1);
  nrf_tx(cmdf,3);
  while(nrf_tx_state() == NRF_TXING);

  /*************虚拟示波器（看PID）************/
  nrf_tx_buff[1]=(uint8)fabs(10*Motor_Left.Pulse_Count);
  nrf_tx_buff[0]=2;
  nrf_tx(nrf_tx_buff,2);
  /******************************************/
  /***************无线获取图像****************/
  /*di=0;
   for(int i=0;i<20;i++)
  {
    for(int j=0;j<30;j++)
    {
      nrf_tx_buff[1+j]=(img.img_bin_buff[j+30*i]);
      di++;
    }
    nrf_tx(nrf_tx_buff,31);
    while(nrf_tx_state() == NRF_TXING);
  }     */
  /*****************************************/

  while(nrf_tx_state() == NRF_TXING);
  nrf_tx(cmdr,3);
  while(nrf_tx_state() == NRF_TXING);
  di=pit_time_get(PIT1);   //


}


/******************* (C) COPYRIGHT 2018 Home of Innovation *****END OF FILE****/