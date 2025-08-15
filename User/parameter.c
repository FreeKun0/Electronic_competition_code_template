#include "parameter.h"

Param_InitTypeDef param;
Flag_InitTypeDef  flag;
Count_InitTypeDef count;

void reset(void)
{
	__set_FAULTMASK(1); //关闭所有中断
	NVIC_SystemReset(); //复位
}
float Limit_float(float num, float upper, float lower)
{
	num = (num > upper) ? upper : ((num < lower) ? lower : num);
	return num;
}
float Limit_int16_t(int16_t num, int16_t upper, int16_t lower)
{
	num = (num > upper) ? upper : ((num < lower) ? lower : num);
	return num;
}
float Limit_uint16_t(uint16_t num, uint16_t upper, uint16_t lower)
{
	num = (num > upper) ? upper : ((num < lower) ? lower : num);
	return num;
}
float Limit_int32_t(int32_t num, int32_t upper, int32_t lower)
{
	num = (num > upper) ? upper : ((num < lower) ? lower : num);
	return num;
}
float Limit_uint32_t(uint32_t num, uint32_t upper, uint32_t lower)
{
	num = (num > upper) ? upper : ((num < lower) ? lower : num);
	return num;
}
