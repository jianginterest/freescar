#include<include.h>



static int display_flag=0; //�Ƿ���ʾͼ���־λ
uint8 Turn_Direction=0;
#define forward   25



/*************************��С�� ����� ��С�� �����  *******/
struct IMG img={1,2,40,40,  10,    55,     3,    76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  /**
 * @brief      ����洢����ͼ�������
 * @param  	None
 * @retval	None
 */
//uint8 img[CAMERA_H][CAMERA_W];  //�Ҷ�����
/**
 * @brief       ��ʼ������ͷ��ʾλ�ã����أ���ʾ��С
 * @param  	None
 * @retval	None
 */
    Site_t site     = {0, 0};                   //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};    //ͼ���С[80,60]
    Size_t size={80,60};                   //��ʾ����ͼ���С



 /**
 * @brief   ��׼����
 * @param  	15~30
 * @retval	None
 */
  uint8 Min_Margin_standard[60]=
  {
    15,15,15,15,15,  15,15,15,15,15, 15,15,15,15,15,
    15,15,15,15,15,  19,23,27,32,36, 40,45,50,54,58,
    62,67,67,67,71,  71,71,71,71,71, 71,71,71,71,71,
    71,71,71,71,71,  71,71,71,71,71, 71,71,71,71,71

  };

  uint8 Left_Margin_standard[60]=
   {  3,3,3,3,3,  3,3,3,3,3,  3,3,3,3,3,

      3,3,37,32,30,  28,26,23,21,20,  17,15,13,11,9,
      7,5, 3,3, 3,   3, 3, 3, 3,  3,   3, 3, 3, 3, 3,
      3,3, 3,3, 3,   3, 3, 3, 3,  3,   3, 3, 3, 3, 3,
   };
   uint8 Right_Margin_standard[60]=
   { 78,78,78,78,78,  78,78,78,78,78,  78,78,78,78,78,    //14

     48,40,40,43,46,  48,51,53,55,57,  60,62,64,66,69,     //29
     71,73,75,73,78,  78,78,78,78,78,  78,78,78,78,78,
     78,78,78,78,78,  78,78,78,78,78,  78,78,78,78,78


   };




/**
 * @brief   ������С���˷����ýṹ��
 * @param  	None
 * @retval	None
 */
struct least_square_method{
    uint8 X[70];
    float a;
    float b;
    float Average_x;
    float Average_y;
    float Sum_xy;
    float Sum_x;
    float Sum_y;
    float Sum_xx;
}LSM={0,0,0,0,0,0,0,0,0,0 };

/**
 * @brief       ��ʾ������+����ͷ��ȡͼ�񣨲���ʾ��
 * @param  	None
 * @retval	None
 */

void LCD_camera_init(void)
{
    LCD_init();
    size.H = LCD_H;
    size.W = LCD_W;
    LCD_rectangle((Site_t){2,107},(Size_t){ 124,1}, RED);
    LCD_str((Site_t){6,110},"speed ",WHITE,BLACK);

    LCD_str((Site_t){40,4},"marvelous! ",WHITE,BLACK);
    camera_init(img.img_bin_buff);
    /*�����жϷ�����   */
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler

    //�ȶ����鸳��ֵ  ����������ߵĺ�����
    for(img.Count=0;img.Count<70;img.Count++){
        img.Road_Middle[img.Count]=40;
         img.Left_Margin[img.Count]=3;
         img.Right_Margin[img.Count]=76;
        LSM.X[img.Count]=img.Count;
    }
}

/**
 * @brief   ��LCD�Ͻ��ڰ׵����
 * @param  	None
 * @retval	None
 */
void LCD_image_display()
{
  Site_t site;
    for(site.x=0;site.x<=80;site.x++)//��Һ������ͼ��
    {
      for(site.y=0;site.y<=60;site.y++)
      {
        if(img.imgbuff[site.y][site.x]==0)
        {
          LCD_point(site,BLACK);
        }
        if(img.imgbuff[site.y][site.x]==255)
        {
          LCD_point(site,WHITE);
        }
         if(img.imgbuff[site.y][site.x]==120)
        {
          LCD_point(site,YELLOW);
        }
        if(img.imgbuff[site.y][site.x]==140)
        {
          LCD_point(site,GREEN);
        }

         if(img.imgbuff[site.y][site.x]==150)
        {
          LCD_point(site,RED);
        }
         if(img.imgbuff[site.y][site.x]==160)
        {
          LCD_point(site,GREEN);
        }
         if(img.imgbuff[site.y][site.x]==170)
        {
          LCD_point(site,BLUE);
        }


    }
  }
}
/**
 * @brief   ͨ������ѡ���Ƿ���ʾͼ�������ʾ���Ҽ��ر�ͼ��
 * @param  	None
 * @retval	None
 */
void key_image()
{
  if(key_get(KEY_L) ==  KEY_DOWN)
  {
      display_flag=1;
  }
  else if(key_get(KEY_R) ==  KEY_DOWN)
  {  display_flag=2;
  }
  else if(key_get(KEY_U) ==  KEY_DOWN)
  {  display_flag=4;
  }
  else if(key_get(KEY_D) ==  KEY_DOWN)
  {
    if(display_flag==4)
    display_flag=3;
  }
  if(display_flag==1)
  {

    //������ǰͼ��ɨ���к�ͼ������40
    //LCD_rectangle((Site_t){80,  img.Row_Min},(Size_t){ 40, 1}, WHITE);
   // LCD_rectangle((Site_t){80,  img.Row_Max},(Size_t){ 40, 1}, WHITE);
    LCD_rectangle((Site_t){40, 1}           ,(Size_t){ 1, 80}, WHITE);
    //if(img.rotary!=1)
    //{
  //  for(img.Count=img.Row_Max-1;img.Count>=img.Row_Max-img.Effective_Row;img.Count--){
    //        img.Road_Middle[img.Count]=(uint8)(LSM.a*LSM.X[img.Count]+LSM.b);
   //    }  //�������¼���
   // }
        LCD_image_display();
        for(img.Count=img.Row_Max-1; img.Count >=img.Row_Max-img.Effective_Row; img.Count--){

            if(img.Road_Middle[img.Count]>78){
                img.Road_Middle[img.Count]=78;
            }else if(img.Road_Middle[img.Count]<1){
                img.Road_Middle[img.Count]=1;
            }
  }


  }
  else if(display_flag==2)
  {
    LCD_camera_init();

    display_flag=0;    //��ֹ����������д
  }
  else if(display_flag==3)
  {
    flash_erase_sector(SECTOR_NUM);
    for(int i=0;i<21;i++)
    {
      flash_write(SECTOR_NUM,200+i*4,(img.Right_Margin[15+i]-img.Left_Margin[15+i]-1));
    }

    LCD_str((Site_t){40,60},"complete! ",WHITE,BLACK);
    DELAY_MS(500);
    display_flag=1;

  }
}

/**
 * @brief   ͼ���Ե(margin)������
 * @param  	����ͼ���ά�����
 * @retval	��⵽��⵽�����ң�����ɨ������1,����ɨ������2,���Ǳ�Ե����0
 */
uint8 Check_Margin(uint8 Row,uint8 Col){           //255��
    if((img.imgbuff[Row][Col-1]==0) && (img.imgbuff[Row][Col]==0) && (img.imgbuff[Row][Col+1]==255)){
        return img.Left_Jump;  //��������ɨ��
    }else if((img.imgbuff[Row][Col-1]==255) && (img.imgbuff[Row][Col]==255) && (img.imgbuff[Row][Col+1]==0)){
        return img.Right_Jump; //��������ɨ��
    }else{
        return 0;
    }
}

uint8 Check_L(uint8 Row,uint8 Col)
{
  if(((img.imgbuff[Row][Col-1]+img.imgbuff[Row][Col])==0) && ((img.imgbuff[Row][Col+1]+img.imgbuff[Row][Col+2])==510)) return 1;
  else return 0;
}

uint8 Check_R(uint8 Row,uint8 Col)
{
  if(((img.imgbuff[Row][Col-1]+img.imgbuff[Row][Col-2])==510) && ((img.imgbuff[Row][Col+1]+img.imgbuff[Row][Col])==0)) return 1;
  else return 0;
}
/**
 * @brief       ɨ������
 * @param  	None
 * @retval	None
 */
 int i,j;
  void Scan_Img_Array(void){

   /****��ǰ�����ֺ��У����ݲɼ����� ��ͣ�� Stop ****/

    if( (img.imgbuff[50][40]==0)   &&
        (img.imgbuff[50][40+1]==0) &&
        (img.imgbuff[50][40+2]==0) &&
        (img.imgbuff[50][40+3]==0) &&
        (img.imgbuff[50][40+4]==0) &&
        (img.imgbuff[50][40-1]==0) &&
        (img.imgbuff[50][40-2]==0) &&
        (img.imgbuff[50][40-3]==0) &&
        (img.imgbuff[50][40-4]==0)
    ){
     // ftm_pwm_duty(FTM0, FTM_CH4,0 );
     // ftm_pwm_duty(FTM0, FTM_CH5,0 );
     // ftm_pwm_duty(FTM0, FTM_CH6,0 );
      //ftm_pwm_duty(FTM0, FTM_CH7,0 );
    }

    /******************* ����ͼ��ɨ������  *******************/

    img.Mid=40;                 //������ʼɨ���е�
    img.Effective_Row=0;        //����������Ч��

    img.left_line=0;
    img.right_line=0;


    img.first=0;
    img.number1=0;
    img.number2=0;
    img.number3=0;
    img.number4=0;
    img.R=0;
    img.L=0;

    img.l_sum=0;
    img.r_sum=0;

/******************* ����ͼ��ɨ�����ݽ���  *******************/
  img.Left_Margin[img.Row_Max]=3;
  img.Road_Middle[img.Row_Max]=39;
  img.Right_Margin[img.Row_Max]=76;
  img.x1=img.Row_Max;

 for(img.row=img.Row_Max-1;img.row>=img.Row_Min;img.row--){
    //�ȶ�����ɨ��㸳��ֵ�����ж�������ǰ���

        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Road_Middle[img.row]=img.Road_Middle[img.row+1];
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
         img.left_flag[img.row]=0;
        img.right_flag[img.row]=0;



                 //255Ϊ��

     /***************************�״�Ѱ�ұ߽�******************************/
   if(img.first==0)
    {
      for(img.col=img.Mid; img.col>=img.col_Min;img.col--)
      {
        if(Check_L(img.row,img.col))
        {
          img.Left_Margin[img.row]=img.col;
          img.left_flag[img.row]=1;
          break;
        }
      } //��߽�ɨ�����
      for(img.col=img.Mid; img.col<=img.col_Max; img.col++)
      {
        if(Check_R(img.row,img.col))
        {
          img.Right_Margin[img.row] = img.col;
          img.right_flag[img.row]=2;
          break;
        }
      } //�ұ߽�ɨ�����
      img.first=img.right_flag[img.row]+img.left_flag[img.row];
    }
    else
    {
      for(img.col=img.Left_Margin[img.row+1]; img.col<img.Right_Margin[img.row+1];img.col++)
      {
        if(Check_L(img.row,img.col))
        {
          img.Left_Margin[img.row]=img.col;
          img.left_flag[img.row]=1;
          break;
        }
      } //��߽�ɨ�����
      for(img.col=img.Right_Margin[img.row+1]; img.col>img.Left_Margin[img.row+1]; img.col--)
      {
        if(Check_R(img.row,img.col))
        {
          img.Right_Margin[img.row] = img.col;
          img.right_flag[img.row]=2;
          break;
        }
      } //�ұ߽�ɨ�����
      switch (img.left_flag[img.row]+img.right_flag[img.row])
      {
      case 0://���Ҷ�û�ҵ�
        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
        img.Mid = img.Road_Middle[img.row];
        break;
      case 1:       //���ҵ�
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)(img.Left_Margin[img.row]+(Min_Margin_standard[img.row]/2));
        img.Mid = img.Road_Middle[img.row];
        break;
      case 2:           //���ҵ�
        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)(img.Right_Margin[img.row]-(Min_Margin_standard[img.row]/2));
        img.Mid = img.Road_Middle[img.row];
        break;
      case 3:                //���ҵ�
        img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
        img.Mid = img.Road_Middle[img.row];
        break;
      }

      if((img.Mid>65)||(img.Mid<15))
      {
        if(img.row>forward)       img.Road_Middle[forward]  =  img.Mid;
         break;  //�����̫Զ���������ֱ������ѭ��˵��ɨ�赽��ͼ����ǰ��
      }

   }

      img.imgbuff[img.row][img.Left_Margin[img.row]+1]=150;
      img.imgbuff[img.row][img.Right_Margin[img.row]-1]=170;
      img.imgbuff[img.row][img.Road_Middle[img.row]]=160;
       if((img.row>19)&&(img.row<29))
       {
       img.r_sum+= abs(img.Right_Margin[img.row]-Right_Margin_standard[img.row]);
       img.l_sum+=abs(img.Left_Margin[img.row]-Left_Margin_standard[img.row]);
       }

       if(img.row==forward)
       {
         for(int k=0;k<80;k++)
           img.imgbuff[forward][k]=150;

       }

   }
 //----------------------------------------------------------------//
 //----------------------------�󻷵�-----------------------------//
/*  img.rotary=0  ��ʾû�н��뻷��
    img.rotary=3  ��������
    img.rotary=1  ��ʼ���뻷���������� �̶���
    img.rotary_in=1   �ڻ�����
*/
 if(img.rotary==0)
 {
 for(img.row=img.Row_Max-1;img.row>img.Row_Min;img.row--) //**
 {
  if((img.Left_Margin[img.row]==3)||(img.Left_Margin[img.row]==1))  //���������߽�δ�ҵ���ɨ��һ��
  {continue;}
  else if(img.imgbuff[img.row-9][2]==255 ) //���������߽��ҵ���������9�еڶ����ǰ�ɫ����˼��ɨ�����󻷵����С��9�У�
  {
    for(i=img.row;i>img.row-8;i--)         //���ҵ��߽����һ�У�����ɨ8�У���8������߽磩
  {
    for(j=0;j<img.Left_Margin[i];j++)     //��0ɨ����߽磬������ɫ����Ĵ�С
  {
     if(img.imgbuff[i][j]==0)
     {
       img.imgbuff[i][j]=120;        //����Ǻ�ɫ�͸�ֵΪ��ɫ
       img.number1++;                //��ɫ�����С��һ
     }

  }
  }
  if((img.number1<55)&&( img.number1>20)&&(abs(img.Error)<5))  //�����ɫ�����СС��55 �� ����20 �Ҷ�����С��5
  {

  for(img.row=0;img.row<img.Row_Max;img.row++)  //*
  {
    if((img.imgbuff[img.row][0]==120)&&(img.imgbuff[img.row][1]==120)) //Ѱ�һ�ɫ���򣬼�Ѱ�һ������·���һ��
    {

     for(i=img.row;i>img.row-8;i--)
     {

       if(img.left_flag[i]==1)               //�ҵ��������Ϸ���һ��
       {     continue; }
        else if (img.right_flag[i]==2)    //�жϻ������Ϸ���һ�� �Ƿ����ұ߽�
       {  img.number2++;              //������ұ߽� img.number2++����ʾ�����Ϸ� ��ඪ�� �Ҳ��б߽����� �ж�����
         continue;
       }
     }
     break; //����forѭ�� ��ע�� *���Ǹ���
    }
  }
  }

   if(img.number2>3)                    //��������Ϸ���ඪ�� �Ҳ��ұ߽���г���3��
   {
     img.number2=0;
     img.number1=0;
     gpio_init(PTC14,GPO,0);    //��ɫ ����
     for(img.row=1;img.row<img.Row_Max;img.row++)    //���ϵ��£��������ұ߽�ĵ�һ�У������ұ߽����Զ�У�
    {
      if( img.right_flag[img.row]==2)
        {
          img.right_line=img.row;
          break;
        }
          }

    for(img.row=img.right_line;img.row<30;img.row++)  //�����ұ߽�������30�У����������ұ߽��ж���
    {
     if(img.right_flag[img.row]==2)
      { img.R++;}
     else {break;}
      }

     for(img.row=img.right_line;img.row<30;img.row++)   //�����ұ߽����Զ������ɨ30�У�����������߽��ж���
       {
        if(img.left_flag[img.row]==1)
        {img.L++;}
        else {break;}
        }
    if((img.R-img.L)>6)  //����������ұ߽����������߽��6�У��ж�Ϊ����
    {
      gpio_init(PTC15,GPO,0);       //��ɫ ����
      img.rotary=3;                  //��������
      distance=0;                   //��ʼ�Ǳ�����50cm���ڱ����������ģ�
         }

   }
   break;         //����ע��**��forѭ��

 }            //else if���� ����


  /**************************������**********************/
 /*
       if(img.rotary==2)  //��������
       { for(img.row=img.Row_Max;img.row>img.Row_Min;img.row--)
         { if((img.Right_Margin[img.row]!=78)&&(img.Right_Margin[img.row-1]<img.Right_Margin[img.row]))
             {
               img.x1=img.row+1;
               img.y1=78;
               img.x2=img.Row_Max;
               img.y2=img.col_Min;
               img.k=(img.y2-img.y1)/(img.x2-img.x1);
               img.b=img.y2-(img.k*img.x2);
               break;
              }
         }
         for(img.row=img.x1;img.row>img.Row_Min;img.row--)
         {
          img.Right_Margin[img.row]=78;
          img.Left_Margin[img.row]=78;
         }
         for(img.row=img.Row_Max;img.row>img.x1;img.row--)
         {
         img.Left_Margin[img.row]=(int)((img.k*img.row)+img.b);
         img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
         img.Mid = img.Road_Middle[img.row];
         }



       }else     */


  //-------------------------------------------------------------------------//
 //----------------------------------�һ���---------------------------------//
  /*
 if(img.rotary==0)
 {
    for(img.row=img.Row_Max-1;img.row>img.Row_Min;img.row--)
 {
  if((img.Right_Margin[img.row]==76)||(img.Right_Margin[img.row]==78))
  {continue;}
  else
  {
    for(i=img.row;i>img.row-8;i--)
  {
    for(j=78;j>img.Right_Margin[i];j--)
  {
     if(img.imgbuff[i][j]==0)
     {
       img.imgbuff[i][j]=140;
       img.number3++;
     }
  }

  }
  if((img.number3<40)&&( img.number3!=0))
  {

  for(img.row=0;img.row<img.Row_Max;img.row++)
  {
    if((img.imgbuff[img.row][77]==140)&&(img.imgbuff[img.row][76]==140))
    {

     for(i=img.row;i>img.row-8;i--)
     {
       if(img.right_flag[img.row]==3)
       {     continue; }
        else if (img.left_flag[img.row]==1)
       {  img.number4++;}
       else
       {  continue; }
     }
       break;
    }

  }
  }

   if(img.number4>4)
   {
     img.number3=0;
     img.number4=0;
     gpio_init(PTC15,GPO,0);    //��ɫ 0
     for(img.row=1;img.row<img.Row_Max;img.row++)
    {
      if( img.left_flag[img.row]==1)
        {
          img.left_line=img.row;
          break;
        }
          }

    for(img.row=img.left_line;img.row<30;img.row++)
    {
     if(img.right_flag[img.row]==3)
      { img.R++;}
     else {break;}
      }

     for(img.row=img.left_line;img.row<30;img.row++)
       {
        if(img.left_flag[img.row]==1)
        {img.L++;}
        else {break;}
        }
    if((img.L-img.R)>6)
    {
      gpio_init(PTC14,GPO,0);       //��ɫ0
      img.rotary=2;
      distance=0;
         }

   }
   break;
  }

 }

if((img.rotary==2)&&(distance>3500))  //(3730)��������(0����)
  {
    gpio_init(PTC15,GPO,1);    //��ɫ 0
    img.Error=18;
    distance=0;
   }



  }
 */
   }    //forѭ������
 }      // if(img.rotary==0)����


 //---ɨ���󻷵���ͼ���� ---//
 /*
  if(img.rotary==3)
  {
    for(img.row=img.Row_Max-2;img.row>img.Row_Min;img.row--)
    {
     if(img.Left_Margin[img.row]>img.Left_Margin[img.row+1])
     {
       img.x2=img.row;
       img.y2=img.Right_Margin[img.row];
     }
    }
    img.x1=img.x2-15;
    if(img.x1<0)
    {img.x1=1;}
    img.y1=1;

    img.k=(img.y2-img.y1)/(img.x2-img.x1);
    img.b=img.y2-(img.k*img.x2);
    for(img.row=img.x2;img.row>img.x1;img.row--)
    {
      img.Right_Margin[img.row]=(int)((img.k*img.row)+img.b);
      img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
    }


  }
         */


  if((img.rotary==3)&&(distance>3000))  //(3730)��������(0����)     //�����������50cm
  {
    gpio_init(PTC14,GPO,1);    //��ɫ 0
    img.rotary=1;             //���뻷��
    img.Error=-18;            //������

     distance=0;              //���¼�����루�ӳ���  Calc_Track_Error������
   }


  }


 /**
 * @brief   ���������˲�����С���˷�
 *          a=( sum(x*y)- sum(x)*sum(y)/n )/ ( sum(x*x)- sum(x)*sum(x)/n )
 *          b= average(y)- a*average(x);
 *          y=a*x+b;
 *          a��б��  b�ǽؾ� x�Ƕ�Ӧ�� y�Ƕ�Ӧ������ֵ
 * @param  	����Ϊy������Ϊ�У�������ߵĺ���
 * @retval	None
 */
void Road_Data_Filter(){
    LSM.Sum_x  =0;
    LSM.Sum_y  =0;
    LSM.Sum_xy =0;
    LSM.Sum_xx =0;

    for(img.Count=img.Row_Max-1;img.Count>=img.Row_Max-img.Effective_Row;img.Count--){

        LSM.Sum_x  += LSM.X[img.Count];
        LSM.Sum_y  += img.Road_Middle[img.Count];
        LSM.Sum_xy += img.Road_Middle[img.Count] * LSM.X[img.Count];
        LSM.Sum_xx += LSM.X[img.Count] * LSM.X[img.Count];
    }

    LSM.Average_x=LSM.Sum_x/ img.Effective_Row;
    LSM.Average_y=LSM.Sum_y/ img.Effective_Row;

    LSM.a=(LSM.Sum_xy - (LSM.Sum_x*LSM.Sum_y / img.Effective_Row ) )/
          (LSM.Sum_xx - (LSM.Sum_x*LSM.Sum_x / img.Effective_Row ) );
    LSM.b=LSM.Average_y- LSM.a*LSM.Average_x;
}


/**
 * @brief   ������������ֵ��img.Error���ƶ��
 * @param  	None
 * @retval	None
 */
uint8 flag,a,b;
void Calc_Track_Error(void){
  img.r_lost=-1;
  img.l_lost=-1;
  a=0;
  b=0;

  if(img.rotary!=1)            //�Ƕ���̶���ǽ׶�  ���˶γ������ڳ�������
  {
    for(img.row=img.Row_Max;img.row>img.Row_Min;img.row--)
    {
      if((a==0)&&(img.right_flag[img.row]==0))   // �Ҳඪ�� a=0��ʾ������е���һ�д�û�й��߽�
      { img.right_line++;  }                     // right_line++
       else if(img.right_flag[img.row]==2)     //����Ҳ಻����
       {
         if(abs(img.Right_Margin[img.row]-img.Right_Margin[img.row-1])>10)
         {                              //�Ҳ��������б߽���У�������10��
           img.r_lost++;
         } else
         {img.r_lost--;}         //�������10��r_lost++�����������10����r_lost--
         a=1;                      //��ʾ�й��߽���
       }
      if((b==0)&&(img.left_flag[img.row]==0))  //��ඪ�ߣ��˶γ������Ϸ����Ҳ����һ��
      {img.left_line++;}
      else if((img.left_flag[img.row]==1))
      {
         if(abs(img.Left_Margin[img.row]-img.Left_Margin[img.row-1])>10)
         {
           img.l_lost++;
         }
      else
      { img.l_lost--;}
      b=1;

      }
    }


    if( (  ((img.left_line>25)&&(img.right_line>25))    //��� ��ඪ����������25�����Ҳ�Ҳ����25
       ||((img.left_line>40)&&(img.right_line>20))       //������ඪ�ߴ���40 ���Ҳ����20
       ||((img.left_line>20)&&(img.right_line>40))  )     //������ඪ�ߴ���20 ���Ҳ����40

       && (abs(img.left_line-img.right_line)<19)          //���С��19
       // &&((img.imgbuff[5][39]==200)||(img.imgbuff[5][39]==255)    )
       //   &&((img.imgbuff[10][39]==200)||(img.imgbuff[10][39]==255) )
     // &&( img.rotary_in==1 )
       &&((abs(img.r_lost)<2)||(abs(img.l_lost)<2)||(img.r_lost==0)||(img.l_lost==0))) //�����ڱ߽��ֵ����10���б�С��10�Ķ�
    {                                       //�ж����ڳ����������̶����
        //img.Error=img.last_error;
      if(img.last_error==2)
      {
        img.Error=-20;}
      else if(img.last_error==1){
        img.Error=20;}
       img.rotary_in=0;
    }

    else             //��������
    {
    img.Error= img.Road_Middle[forward] -39;

     //��־�������ջ����ҹ�
    if(img.Error>0)         //�ҹ�
    {img.last_error=1;   }
    else if(img.Error<0)   //���
    {img.last_error=2;}

    }

  }   // if(img.rotary!=1) ����


  else if((img.rotary==1)&&(distance>2000))    //�������̶���ǣ��� ���뵽��2000����
  {
    img.rotary=0;                 //��־λ���� ���뻷������
    gpio_init(PTC15,GPO,1);    //��ɫ ����
    img.rotary_in=1;            //�ڻ�����
  }
}





