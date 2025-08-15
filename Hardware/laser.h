#ifndef __LASER_H
#define __LASER_H

#include "stm32f10x.h"                  // Device header
//#include "parameter.h"
#include "Delay.h"


#define Laser_Purple_GPIO_Pin 		GPIO_Pin_3
#define Laser_Purple_GPIO_Port 		GPIOB
#define Laser_Purple_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)



void Laser_P_Init(void);

void Laser_P_ON(void);
void Laser_P_OFF(void);
void Laser_P_Turn(void);


#endif
