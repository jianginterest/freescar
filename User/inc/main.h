#ifndef _MAIN_H_
#define _MAIN_H_

/**
 * @brief   这个数值的不同，可以选择，
 *          1：为图像，2为现行CCD，3为虚拟示波器
 * @param  解释在computer.c中
 * @retval	None
 */
#define CMD_IMG 1


extern int16 Num_Pluse;
extern int16 distance;
void labview(void);

#endif