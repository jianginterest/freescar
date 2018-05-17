#include<include.h>



static int display_flag=0; //是否显示图像标志位
uint8 Turn_Direction=0;
#define forward   25



/*************************最小行 最大行 最小列 最大列  *******/
struct IMG img={1,2,40,40,  10,    55,     3,    76,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
 * @brief   标准数组
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

    //画出当前图像扫描行和图像中线40
    //LCD_rectangle((Site_t){80,  img.Row_Min},(Size_t){ 40, 1}, WHITE);
   // LCD_rectangle((Site_t){80,  img.Row_Max},(Size_t){ 40, 1}, WHITE);
    LCD_rectangle((Site_t){40, 1}           ,(Size_t){ 1, 80}, WHITE);
    //if(img.rotary!=1)
    //{
  //  for(img.Count=img.Row_Max-1;img.Count>=img.Row_Max-img.Effective_Row;img.Count--){
    //        img.Road_Middle[img.Count]=(uint8)(LSM.a*LSM.X[img.Count]+LSM.b);
   //    }  //中线重新计算
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

    display_flag=0;    //防止总是清屏重写
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
 * @brief       扫描赛道
 * @param  	None
 * @retval	None
 */
 int i,j;
  void Scan_Img_Array(void){

   /****最前方出现黑行，数据采集错误 ，停车 Stop ****/

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

    /******************* 重置图像扫描数据  *******************/

    img.Mid=40;                 //重置起始扫描中点
    img.Effective_Row=0;        //重置赛道有效行

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

/******************* 重置图像扫描数据结束  *******************/
  img.Left_Margin[img.Row_Max]=3;
  img.Road_Middle[img.Row_Max]=39;
  img.Right_Margin[img.Row_Max]=76;
  img.x1=img.Row_Max;

 for(img.row=img.Row_Max-1;img.row>=img.Row_Min;img.row--){
    //先对赛道扫描点赋初值，可判断赛道当前情况

        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Road_Middle[img.row]=img.Road_Middle[img.row+1];
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
         img.left_flag[img.row]=0;
        img.right_flag[img.row]=0;



                 //255为白

     /***************************首次寻找边界******************************/
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
      } //左边界扫描结束
      for(img.col=img.Mid; img.col<=img.col_Max; img.col++)
      {
        if(Check_R(img.row,img.col))
        {
          img.Right_Margin[img.row] = img.col;
          img.right_flag[img.row]=2;
          break;
        }
      } //右边界扫描结束
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
      } //左边界扫描结束
      for(img.col=img.Right_Margin[img.row+1]; img.col>img.Left_Margin[img.row+1]; img.col--)
      {
        if(Check_R(img.row,img.col))
        {
          img.Right_Margin[img.row] = img.col;
          img.right_flag[img.row]=2;
          break;
        }
      } //右边界扫描结束
      switch (img.left_flag[img.row]+img.right_flag[img.row])
      {
      case 0://左右都没找到
        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
        img.Mid = img.Road_Middle[img.row];
        break;
      case 1:       //左找到
        img.Right_Margin[img.row]=img.Right_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)(img.Left_Margin[img.row]+(Min_Margin_standard[img.row]/2));
        img.Mid = img.Road_Middle[img.row];
        break;
      case 2:           //右找到
        img.Left_Margin[img.row]=img.Left_Margin[img.row+1];
        img.Road_Middle[img.row]=(uint8)(img.Right_Margin[img.row]-(Min_Margin_standard[img.row]/2));
        img.Mid = img.Road_Middle[img.row];
        break;
      case 3:                //都找到
        img.Road_Middle[img.row]=(uint8)((img.Right_Margin[img.row]+img.Left_Margin[img.row])/2);
        img.Mid = img.Road_Middle[img.row];
        break;
      }

      if((img.Mid>65)||(img.Mid<15))
      {
        if(img.row>forward)       img.Road_Middle[forward]  =  img.Mid;
         break;  //计算点太远，会溢出，直接跳出循环说明扫描到了图像最前方
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
 //----------------------------左环岛-----------------------------//
/*  img.rotary=0  表示没有进入环岛
    img.rotary=3  看到环岛
    img.rotary=1  开始进入环岛（舵机打角 固定）
    img.rotary_in=1   在环岛中
*/
 if(img.rotary==0)
 {
 for(img.row=img.Row_Max-1;img.row>img.Row_Min;img.row--) //**
 {
  if((img.Left_Margin[img.row]==3)||(img.Left_Margin[img.row]==1))  //如果这行左边界未找到，扫下一行
  {continue;}
  else if(img.imgbuff[img.row-9][2]==255 ) //如果这行左边界找到，而且上9行第二列是白色（意思是扫到的左环岛宽度小于9行）
  {
    for(i=img.row;i>img.row-8;i--)         //从找到边界的这一行，往上扫8行（这8行有左边界）
  {
    for(j=0;j<img.Left_Margin[i];j++)     //从0扫到左边界，即数黑色区域的大小
  {
     if(img.imgbuff[i][j]==0)
     {
       img.imgbuff[i][j]=120;        //如果是黑色就赋值为黄色
       img.number1++;                //黑色区域大小加一
     }

  }
  }
  if((img.number1<55)&&( img.number1>20)&&(abs(img.Error)<5))  //如果黑色区域大小小于55 且 大于20 且舵机误差小于5
  {

  for(img.row=0;img.row<img.Row_Max;img.row++)  //*
  {
    if((img.imgbuff[img.row][0]==120)&&(img.imgbuff[img.row][1]==120)) //寻找黄色区域，即寻找环岛最下方那一行
    {

     for(i=img.row;i>img.row-8;i--)
     {

       if(img.left_flag[i]==1)               //找到环岛最上方那一行
       {     continue; }
        else if (img.right_flag[i]==2)    //判断换到最上方上一行 是否有右边界
       {  img.number2++;              //如果有右边界 img.number2++，表示环岛上方 左侧丢线 右侧有边界的情况 有多少行
         continue;
       }
     }
     break; //跳出for循环 （注释 *的那个）
    }
  }
  }

   if(img.number2>3)                    //如果环岛上方左侧丢线 右侧右边界的行超过3行
   {
     img.number2=0;
     img.number1=0;
     gpio_init(PTC14,GPO,0);    //蓝色 灯亮
     for(img.row=1;img.row<img.Row_Max;img.row++)    //从上到下，找有有右边界的第一行（即有右边界的最远行）
    {
      if( img.right_flag[img.row]==2)
        {
          img.right_line=img.row;
          break;
        }
          }

    for(img.row=img.right_line;img.row<30;img.row++)  //从有右边界往下数30行，看连续的右边界有多少
    {
     if(img.right_flag[img.row]==2)
      { img.R++;}
     else {break;}
      }

     for(img.row=img.right_line;img.row<30;img.row++)   //从有右边界的最远行往下扫30行，看连续的左边界有多少
       {
        if(img.left_flag[img.row]==1)
        {img.L++;}
        else {break;}
        }
    if((img.R-img.L)>6)  //如果连续的右边界的行数比左边界大6行，判断为环岛
    {
      gpio_init(PTC15,GPO,0);       //绿色 灯亮
      img.rotary=3;                  //看到环岛
      distance=0;                   //开始记编码器50cm（在编码器计数哪）
         }

   }
   break;         //跳出注释**的for循环

 }            //else if（） 结束


  /**************************环岛入**********************/
 /*
       if(img.rotary==2)  //环岛在右
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
 //----------------------------------右环岛---------------------------------//
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
     gpio_init(PTC15,GPO,0);    //绿色 0
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
      gpio_init(PTC14,GPO,0);       //蓝色0
      img.rotary=2;
      distance=0;
         }

   }
   break;
  }

 }

if((img.rotary==2)&&(distance>3500))  //(3730)环岛在左(0在上)
  {
    gpio_init(PTC15,GPO,1);    //绿色 0
    img.Error=18;
    distance=0;
   }



  }
 */
   }    //for循环结束
 }      // if(img.rotary==0)结束


 //---扫到左环岛后图像处理 ---//
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


  if((img.rotary==3)&&(distance>3000))  //(3730)环岛在左(0在上)     //如果计数到达50cm
  {
    gpio_init(PTC14,GPO,1);    //蓝色 0
    img.rotary=1;             //进入环岛
    img.Error=-18;            //舵机打角

     distance=0;              //重新计算距离（接程序  Calc_Track_Error函数）
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
uint8 flag,a,b;
void Calc_Track_Error(void){
  img.r_lost=-1;
  img.l_lost=-1;
  a=0;
  b=0;

  if(img.rotary!=1)            //非舵机固定打角阶段  （此段程序用于出环岛）
  {
    for(img.row=img.Row_Max;img.row>img.Row_Min;img.row--)
    {
      if((a==0)&&(img.right_flag[img.row]==0))   // 右侧丢线 a=0表示从最大行到这一行从没有过边界
      { img.right_line++;  }                     // right_line++
       else if(img.right_flag[img.row]==2)     //如果右侧不丢线
       {
         if(abs(img.Right_Margin[img.row]-img.Right_Margin[img.row-1])>10)
         {                              //右侧相邻两行边界的列，相差多于10列
           img.r_lost++;
         } else
         {img.r_lost--;}         //如果大于10就r_lost++，如果不大于10，就r_lost--
         a=1;                      //表示有过边界了
       }
      if((b==0)&&(img.left_flag[img.row]==0))  //左侧丢线，此段程序与上方有右侧程序一样
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


    if( (  ((img.left_line>25)&&(img.right_line>25))    //如果 左侧丢线行数大于25，且右侧也大于25
       ||((img.left_line>40)&&(img.right_line>20))       //或者左侧丢线大于40 ，右侧大于20
       ||((img.left_line>20)&&(img.right_line>40))  )     //或者左侧丢线大于20 ，右侧大于40

       && (abs(img.left_line-img.right_line)<19)          //相差小于19
       // &&((img.imgbuff[5][39]==200)||(img.imgbuff[5][39]==255)    )
       //   &&((img.imgbuff[10][39]==200)||(img.imgbuff[10][39]==255) )
     // &&( img.rotary_in==1 )
       &&((abs(img.r_lost)<2)||(abs(img.l_lost)<2)||(img.r_lost==0)||(img.l_lost==0))) //且相邻边界差值大于10的行比小于10的多
    {                                       //判断正在出环岛，给固定打角
        //img.Error=img.last_error;
      if(img.last_error==2)
      {
        img.Error=-20;}
      else if(img.last_error==1){
        img.Error=20;}
       img.rotary_in=0;
    }

    else             //正常赛道
    {
    img.Error= img.Road_Middle[forward] -39;

     //标志舵机是左拐还是右拐
    if(img.Error>0)         //右拐
    {img.last_error=1;   }
    else if(img.Error<0)   //左拐
    {img.last_error=2;}

    }

  }   // if(img.rotary!=1) 结束


  else if((img.rotary==1)&&(distance>2000))    //如果舵机固定打角，且 距离到达2000脉冲
  {
    img.rotary=0;                 //标志位清零 进入环岛结束
    gpio_init(PTC15,GPO,1);    //绿色 灯灭
    img.rotary_in=1;            //在环岛中
  }
}





