#include<include.h>



static int display_flag=0; //是否显示图像标志位
uint8 Turn_Direction=0;




/*************************最小行 最大行 最小列 最大列  *******/
struct IMG img={1,2,40,40,  10,    50,     3,    76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  /**
 * @brief      定义存储接收图像的数组
 * @param  	None
 * @retval	None
 */
//uint8 img[CAMERA_H][CAMERA_W];  //灰度数组
/**
 * @brief       初始化摄像头显示位置，像素，显示大小
 * @param  	None
 * @retval	None
 */
    Site_t site     = {0, 0};                   //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H};    //图像大小[80,60]
    Size_t size={80,60};                   //显示区域图像大小



/**
 * @brief   声明最小二乘法所用结构体
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
 * @brief       显示屏配置+摄像头获取图像（不显示）
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
    /*配置中断服务函数   */
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler

    //先对数组赋初值  用于拟合曲线的横坐标
    for(img.Count=0;img.Count<70;img.Count++){
        img.Road_Middle[img.Count]=40;
         img.Left_Margin[img.Count]=3;
         img.Right_Margin[img.Count]=76;
        LSM.X[img.Count]=img.Count;
    }
}

/**
 * @brief   在LCD上将黑白点描出
 * @param  	None
 * @retval	None
 */
void LCD_image_display()
{
  Site_t site;
    for(site.x=0;site.x<=80;site.x++)//给液晶发送图像
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
 * @brief   通过按键选择是否显示图像，左键显示，右键关闭图像
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

    //画出当前图像扫描行和图像中线40
    //LCD_rectangle((Site_t){80,  img.Row_Min},(Size_t){ 40, 1}, WHITE);
   // LCD_rectangle((Site_t){80,  img.Row_Max},(Size_t){ 40, 1}, WHITE);
    LCD_rectangle((Site_t){40, 1}           ,(Size_t){ 1, 80}, WHITE);

    for(img.Count=img.Row_Max-1;img.Count>=img.Row_Max-img.Effective_Row;img.Count--){
            img.Road_Middle[img.Count]=(uint8)(LSM.a*LSM.X[img.Count]+LSM.b);
        }  //中线重新计算

        LCD_image_display();
        for(img.Count=img.Row_Max-1; img.Count >=img.Row_Max-img.Effective_Row; img.Count--){

            if(img.Road_Middle[img.Count]>78){
                img.Road_Middle[img.Count]=78;
            }else if(img.Road_Middle[img.Count]<1){
                img.Road_Middle[img.Count]=1;
            }else{
            }

            //这样主要是为了好观测出现赛道边线，+-1更好的显示边线
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

    display_flag=0;    //防止总是清屏重写
  }
}

/**
 * @brief   图像边缘(margin)跳变检测
 * @param  	检测的图像二维数组点
 * @retval	检测到检测到从左到右，从右扫到返回1,从左扫到返回2,不是边缘返回0
 */
uint8 Check_Margin(uint8 Row,uint8 Col){           //255白
    if((img.imgbuff[Row][Col-1]==0) && (img.imgbuff[Row][Col]==0) && (img.imgbuff[Row][Col+1]==255)){
        return img.Left_Jump;  //从中往左扫描
    }else if((img.imgbuff[Row][Col-1]==255) && (img.imgbuff[Row][Col]==255) && (img.imgbuff[Row][Col+1]==0)){
        return img.Right_Jump; //从中往右扫描
    }else{
        return 0;
    }
}

/**
 * @brief       扫描赛道
 * @param  	None
 * @retval	None
 */

  void Scan_Img_Array(void){

   /****最前方出现黑行，数据采集错误 ，停车 Stop ****/

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

    /******************* 重置图像扫描数据  *******************/

    img.Mid=40;                 //重置起始扫描中点
    img.Effective_Row=0;        //重置赛道有效行
    img.Left_Jump_Flag=0;
    img.Right_Jump_Flag=0;

    img.Left_Jump_Flag=0;
    img.Right_Jump_Flag=0;

    img.Ring_Flag=0;
    img.Ring_Count=0;


/******************* 重置图像扫描数据结束  *******************/


 for(img.row=img.Row_Max-1;img.row>=img.Row_Min;img.row--){
    //先对赛道扫描点赋初值，可判断赛道当前情况
        img.Left_Margin[img.row]=0;
        img.Road_Middle[img.row]=40;
        img.Right_Margin[img.row]=79;
        img.Left_Jump_Count[img.row]=0;
        img.Right_Jump_Count[img.row]=0;

        if((img.Mid>77)||(img.Mid<2)){
            break;  //计算点太远，会溢出，直接跳出循环说明扫描到了图像最前方
        }
                 //255为白
    if((img.imgbuff[img.row][img.Mid]==255)||(img.imgbuff[img.row][img.Mid+15]==255)){
          img.Effective_Row++;    //有效行加一

  //  if(img.imgbuff[img.row][img.Mid]==0){
      //   if(img.imgbuff[img.row][img.Mid+15]==255){
       //         img.Mid=img.Mid+15;
       //         }
       //     }//右拐的时候

     /***************************首次寻找边界******************************/
      if(img.first==1)
      {
   for(img.col=img.Mid; img.col>=img.col_Min;img.col--){
       if(Check_Margin(img.row,img.col)==img.Left_Jump){
           img.Left_Margin[img.row]=img.col;
           break;
             }
         } //左边界扫描结束
    for(img.col=img.Mid; img.col<=img.col_Max; img.col++){
          if(Check_Margin(img.row,img.col) == img.Right_Jump){
             img.Right_Margin[img.row] = img.col;
              break;
                }
          } //右边界扫描结束


     }

          if(img.row<img.Row_Max-1){                    //计算边缘数据有效性

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
     //计算当前行赛道中值
            img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
            img.Mid = img.Road_Middle[img.row];// 前一行扫描起点为上一行中点

 }else{
          break; //结束处理
        }
   }
  }
 /**
 * @brief   赛道数据滤波，最小二乘法
 *          a=( sum(x*y)- sum(x)*sum(y)/n )/ ( sum(x*x)- sum(x)*sum(x)/n )
 *          b= average(y)- a*average(x);
 *          y=a*x+b;
 *          a是斜率  b是截距 x是对应行 y是对应赛道中值
 * @param  	横着为y，竖着为列，求得中线的函数
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
 * @brief   计算赛道误差并赋值给img.Error控制舵机
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






