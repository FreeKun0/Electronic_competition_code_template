#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"                  // Device header
//#include "parameter.h"
#include "Delay.h"


#define Beep_GPIO_Pin 		GPIO_Pin_5
#define Beep_GPIO_Port 		GPIOC
#define Beep_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)



void Beep_Init(void);

void Beep_ON(void);
void Beep_OFF(void);
void Beep_Turn(void);
void Beep_ms(uint32_t xms);


#endif
