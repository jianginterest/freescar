#include<include.h>



static int display_flag=0; //�Ƿ���ʾͼ���־λ
uint8 Turn_Direction=0;




/*************************��С�� ����� ��С�� �����  *******/
struct IMG img={1,2,40,40,  10,    50,     3,    76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
  if(display_flag==1)
  {

    //������ǰͼ��ɨ���к�ͼ������40
    //LCD_rectangle((Site_t){80,  img.Row_Min},(Size_t){ 40, 1}, WHITE);
   // LCD_rectangle((Site_t){80,  img.Row_Max},(Size_t){ 40, 1}, WHITE);
    LCD_rectangle((Site_t){40, 1}           ,(Size_t){ 1, 80}, WHITE);

    for(img.Count=img.Row_Max-1;img.Count>=img.Row_Max-img.Effective_Row;img.Count--){
            img.Road_Middle[img.Count]=(uint8)(LSM.a*LSM.X[img.Count]+LSM.b);
        }  //�������¼���

        LCD_image_display();
        for(img.Count=img.Row_Max-1; img.Count >=img.Row_Max-img.Effective_Row; img.Count--){

            if(img.Road_Middle[img.Count]>78){
                img.Road_Middle[img.Count]=78;
            }else if(img.Road_Middle[img.Count]<1){
                img.Road_Middle[img.Count]=1;
            }else{
            }

            //������Ҫ��Ϊ�˺ù۲�����������ߣ�+-1���õ���ʾ����
            Site_t POINT = {img.Road_Middle[img.Count],img.Count};
            LCD_point(POINT, GREEN);
            POINT.x=img.Left_Margin[img.Count]+1;
            POINT.y=img.Count;
            LCD_point(POINT, RED);
            POINT.x=img.Right_Margin[img.Count]-1;
            POINT.y=img.Count;
            LCD_point(POINT, BLUE);
  }


  }
  else if(display_flag==2)
  {
    LCD_camera_init();

    display_flag=0;    //��ֹ����������д
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

/**
 * @brief       ɨ������
 * @param  	None
 * @retval	None
 */

  void Scan_Img_Array(void){

   /****��ǰ�����ֺ��У����ݲɼ����� ��ͣ�� Stop ****/

    if( (img.imgbuff[30][40]==0)   &&
        (img.imgbuff[30][40+1]==0) &&
        (img.imgbuff[30][40+2]==0) &&
        (img.imgbuff[30][40+3]==0) &&
        (img.imgbuff[30][40+4]==0) &&
        (img.imgbuff[30][40-1]==0) &&
        (img.imgbuff[30][40-2]==0) &&
        (img.imgbuff[30][40-3]==0) &&
        (img.imgbuff[30][40-4]==0)
    ){ motor_init();
    }

    /******************* ����ͼ��ɨ������  *******************/

    img.Mid=40;                 //������ʼɨ���е�
    img.Effective_Row=0;        //����������Ч��
    img.Left_Jump_Flag=0;
    img.Right_Jump_Flag=0;

    img.Left_Jump_Flag=0;
    img.Right_Jump_Flag=0;

    img.Ring_Flag=0;
    img.Ring_Count=0;


/******************* ����ͼ��ɨ�����ݽ���  *******************/


 for(img.row=img.Row_Max-1;img.row>=img.Row_Min;img.row--){
    //�ȶ�����ɨ��㸳��ֵ�����ж�������ǰ���
        img.Left_Margin[img.row]=0;
        img.Road_Middle[img.row]=40;
        img.Right_Margin[img.row]=79;
        img.Left_Jump_Count[img.row]=0;
        img.Right_Jump_Count[img.row]=0;

        if((img.Mid>77)||(img.Mid<2)){
            break;  //�����̫Զ���������ֱ������ѭ��˵��ɨ�赽��ͼ����ǰ��
        }
                 //255Ϊ��
    if((img.imgbuff[img.row][img.Mid]==255)||(img.imgbuff[img.row][img.Mid+15]==255)){
          img.Effective_Row++;    //��Ч�м�һ

  //  if(img.imgbuff[img.row][img.Mid]==0){
      //   if(img.imgbuff[img.row][img.Mid+15]==255){
       //         img.Mid=img.Mid+15;
       //         }
       //     }//�ҹյ�ʱ��

     /***************************�״�Ѱ�ұ߽�******************************/
      if(img.first==1)
      {
   for(img.col=img.Mid; img.col>=img.col_Min;img.col--){
       if(Check_Margin(img.row,img.col)==img.Left_Jump){
           img.Left_Margin[img.row]=img.col;
           break;
             }
         } //��߽�ɨ�����
    for(img.col=img.Mid; img.col<=img.col_Max; img.col++){
          if(Check_Margin(img.row,img.col) == img.Right_Jump){
             img.Right_Margin[img.row] = img.col;
              break;
                }
          } //�ұ߽�ɨ�����


     }

          if(img.row<img.Row_Max-1){                    //�����Ե������Ч��

                if( img.Left_Last_Margin[img.row+1]<img.Left_Margin[img.row]){
                    img.Left_Last_Margin[img.row]=img.Left_Margin[img.row];
                }else{
                    if( (img.Left_Last_Margin[img.row+1]-img.Left_Margin[img.row])>2){
                        img.Left_Margin[img.row]=img.Left_Last_Margin[img.row+1];
                        img.Left_Last_Margin[img.row]=img.Left_Margin[img.row];
                    }else{
                        img.Left_Last_Margin[img.row]=img.Left_Margin[img.row];
                    }
                }

                if( img.Right_Last_Margin[img.row+1]>img.Right_Margin[img.row]){
                    img.Right_Last_Margin[img.row]=img.Right_Margin[img.row];
                }else{
                    if( (img.Right_Margin[img.row]-img.Right_Last_Margin[img.row+1])>2){
                        img.Right_Margin[img.row]=img.Right_Last_Margin[img.row+1];
                        img.Right_Last_Margin[img.row]=img.Right_Margin[img.row];
                    }else{
                        img.Right_Last_Margin[img.row]=img.Right_Margin[img.row];
                    }
                }
            }else{
                img.Left_Last_Margin[img.row]=img.Left_Margin[img.row];
                img.Right_Last_Margin[img.row]=img.Right_Margin[img.row];
            }
     //���㵱ǰ��������ֵ
            img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
            img.Mid = img.Road_Middle[img.row];// ǰһ��ɨ�����Ϊ��һ���е�

 }else{
          break; //��������
        }
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
void Calc_Track_Error(void){

    if(img.Ring_Delay>=3){
       img.Ring_Delay--;
    }

    if(img.Ring_Flag==1){
        img.Ring_Delay=14;
        if(Turn_Direction==1){
            img.Error=-30;
        }else{
             img.Error=30;
        }
    }else{
         Road_Data_Filter();
         img.Error=(int)(LSM.a*25+LSM.b-40);
    }

    if((img.Ring_Flag==0)&& (img.Ring_Delay==2)){

        if((img.Left_Lost_Count>13)&&(Turn_Direction==1)  ){
            img.Ring_Clear=1;
            img.Ring_Delay=0;
        }

        if((img.Right_Lost_Count>13)&&(Turn_Direction==2)  ){
            img.Ring_Clear=2;
            img.Ring_Delay=0;
        }
    }




    if(img.Ring_Flag==0){

        if(img.Ring_Clear==1){
            if(img.Right_Lost_Count<5){
                img.Ring_Clear=0;
            }
            img.Error=-30;
        }

        if(img.Ring_Clear==2){
            if(img.Left_Lost_Count<5){
                img.Ring_Clear=0;
            }
            img.Error=30;
        }
    }


}






