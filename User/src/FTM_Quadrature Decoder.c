#include <include.h>
/**
 * @brief        正交解码配置
 * @param  	None
 * @retval	None
 */
void FTM_Quard(void)
{
  ftm_quad_init(FTM2);         //初始化正交解码口
  pit_config();            //初始化定时器

}