#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
//#include "parameter.h"
//#include "Delay.h"

#define KEY1_GPIO_Port  GPIOA
#define KEY1_GPIO_Pin   GPIO_Pin_11
#define KEY1_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)
				   
#define KEY2_GPIO_Port  GPIOA
#define KEY2_GPIO_Pin   GPIO_Pin_12
#define KEY2_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)				   

#define KEY3_GPIO_Port  GPIOB
#define KEY3_GPIO_Pin   GPIO_Pin_4
#define KEY3_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)	

#define KEY4_GPIO_Port  GPIOB
#define KEY4_GPIO_Pin   GPIO_Pin_5
#define KEY4_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

//#define KEY5_GPIO_Port  GPIOC
//#define KEY5_GPIO_Pin   GPIO_Pin_13
//#define KEY5_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)	

//#define KEY6_GPIO_Port  GPIOC
//#define KEY6_GPIO_Pin   GPIO_Pin_3
//#define KEY6_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)



//#define KEY11_GPIO_Port  GPIOA
//#define KEY11_GPIO_Pin   GPIO_Pin_5
//#define KEY11_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)	

//#define KEY12_GPIO_Port  GPIOB
//#define KEY12_GPIO_Pin   GPIO_Pin_0
//#define KEY12_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

//#define KEY13_GPIO_Port  GPIOB
//#define KEY13_GPIO_Pin   GPIO_Pin_1
//#define KEY13_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)	

//#define KEY14_GPIO_Port  GPIOA
//#define KEY14_GPIO_Pin   GPIO_Pin_1
//#define KEY14_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)	


//#define KEY_RESET_GPIO_Port  GPIOC
//#define KEY_RESET_GPIO_Pin   GPIO_Pin_5
//#define KEY_RESET_GPIO_CLK_ENABLE()		do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)	

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3
#define KEY4_PRES 4





#define KEY5_PRES 5
#define KEY6_PRES 6

#define KEY11_PRES 11
#define KEY12_PRES 12
#define KEY13_PRES 13
#define KEY14_PRES 14
#define KEY15_PRES 15
#define KEY16_PRES 16
#define KEY17_PRES 17
#define KEY18_PRES 18

#define KEY_UP_PRES		KEY11_PRES
#define KEY_DOWN_PRES	KEY12_PRES
#define KEY_LEFT_PRES	KEY13_PRES
#define KEY_RIGHT_PRES	KEY14_PRES

#define KEY_RESET_PRES	99

#define KEY_UP____large_PRES KEY15_PRES
#define KEY_DOWN__large_PRES KEY16_PRES
#define KEY_LEFT__large_PRES KEY17_PRES
#define KEY_RIGHT_large_PRES KEY18_PRES

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif
