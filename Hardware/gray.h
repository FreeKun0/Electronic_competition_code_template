#ifndef __GRAYSENSOR_H
#define __GRAYSENSOR_H

#include "stm32f10x.h"                  // Device header
#include "control.h"
//#include "parameter.h"


#define P_GRAY_PWM_XIAO	250		//PWM，与目标速度限制P_TARGET_SPEED_RPM_LIMIT正相关
#define P_GRAY_PWM_DA	350		//PWM，与目标速度限制P_TARGET_SPEED_RPM_LIMIT正相关
#define P_GRAY_PWM_DA_DA	900

#define GRAY_PWM_XIAO 				P_GRAY_PWM_XIAO		//灰度小的补偿值
#define GRAY_PWM_DA					P_GRAY_PWM_DA		//灰度大的补偿值
#define GRAY_PWM_DA_DA				P_GRAY_PWM_DA_DA		//灰度大的补偿值

#define GARY_L2_GPIO_Pin 	GPIO_Pin_13
#define GARY_L2_GPIO_Port 	GPIOC
#define GARY_L2_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

#define GARY_L1_GPIO_Pin 	GPIO_Pin_0
#define GARY_L1_GPIO_Port 	GPIOC
#define GARY_L1_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

#define GARY_M_GPIO_Pin 	GPIO_Pin_1
#define GARY_M_GPIO_Port 	GPIOC
#define GARY_M_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

#define GARY_R1_GPIO_Pin 	GPIO_Pin_2
#define GARY_R1_GPIO_Port 	GPIOC
#define GARY_R1_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

#define GARY_R2_GPIO_Pin 	GPIO_Pin_3
#define GARY_R2_GPIO_Port 	GPIOC
#define GARY_R2_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

extern uint8_t L2_Val, L1_Val, M_Val, R1_Val, R2_Val;

//extern float Gray_Num;


void gray_Init(void);
void Get_Gray_TTL(void);//内部使用
void Gray_GoStraight_control(void);
float Get_Gray_Num(void);
void Set_Gray_Num(float set_num);

extern uint8_t gw_val[8];
extern uint16_t weights[8];
extern uint16_t weight_sum;
extern uint16_t gw_out_pwm[8];

#define	GWL4	gw_val[0]
#define	GWL3	gw_val[1]
#define	GWL2	gw_val[2]
#define	GWL1	gw_val[3]
#define	GWR1	gw_val[4]
#define	GWR2	gw_val[5]
#define	GWR3	gw_val[6]
#define	GWR4	gw_val[7]


void GW_Gray_GoStraight_control(void);


#endif
