#ifndef _IMAGE_DEAL_H_
#define _IMAGE_DEAL_H_


void sendimg(void *imgaddr, uint32 imgsize); //发送图像到上位机函数
void LCD_camera_init(void);
void key_image();
void LCD_image_display();
//void Least_s_methed(void);
void get_center(void);
uint8 Check_Margin(uint8 Row,uint8 Col);
void Road_Data_Filter();
void Scan_Img_Array(void);
void Calc_Track_Error(void);
void LCD_image_display();



struct IMG
{
   uint8 Right_Jump;
   uint8 Left_Jump;
   uint8 Mid;
   uint8 Last_Mid;
   uint8 Row_Min;
   uint8 Row_Max;
   uint8 col_Min;
   uint8 col_Max;


   uint8  Left_Last_Margin[70];
   uint8  Right_Last_Margin[70];


   uint8 row;
   uint8 col;
   uint8 img_bin_buff[CAMERA_SIZE];    //二值化数组
   //uint8 num;        //二乘法点数
   uint8 imgbuff[60][80];

   uint8 Left_Margin[70];
   uint8 Right_Margin[70];
   int16 Error;
   uint8 Count;
   uint8 Road_Middle[70];
   uint8  Effective_Row;


    uint8  Left_Jump_Count[70];
    uint8  Right_Jump_Count[70];

    uint8  Left_Jump_Flag;
    uint8  Right_Jump_Flag;

    uint8  Ring_Count;
    uint8  Ring_Flag;
    uint8  Ring_Delay;
    uint8  Ring_Clear;

    uint8  first;
    uint8 Left_Lost_Count ;
    uint8 Right_Lost_Count;
};



extern uint8 Turn_Direction;

#endif