#ifndef _IMAGE_DEAL_H_
#define _IMAGE_DEAL_H_


void sendimg(void *imgaddr, uint32 imgsize); //����ͼ����λ������
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
void Tracking(void);



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


   uint8 row;
   uint8 col;
   uint8 img_bin_buff[CAMERA_SIZE];    //��ֵ������
   //uint8 num;        //���˷�����
   uint8 imgbuff[60][80];

   uint8 Left_Margin[70];
   uint8 Right_Margin[70];
   int16 Error;
   int16 last_error;
   uint8 Count;
   uint8 Road_Middle[70];
   uint8 Effective_Row;

   uint8  rotary;      //�Ƿ�ɨ��������־λ
   uint8  first;     //��һ��ɨ���־λ
   uint8  left_flag[60];      //����ҵ��߽��־λ
   uint8  right_flag[60];      //�Ҳ��ҵ��߽��־λ

   uint8  right_line;           //�Ҳඪ�߸���
   uint8  left_line;            //��ඪ�߸���


   int8 x1;
   int8 y1;
   int8 x2;
   int8 y2;
   float k;
   float b;
   uint8 number1;
   uint8 number2;
   uint8 number3;
   uint8 number4;
   int16 L;
   int16 R;
   int8 r_lost;
   int8 l_lost;



};



extern uint8 Turn_Direction;

#endif