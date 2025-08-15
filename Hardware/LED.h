#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

#define LED_RED
//#define LED_GREEN
//#define LED_YELLOW
#define LED_BLUE
#define LED_WHITE

#if defined(LED_RED)
	#define Red_GPIO_Pin 		GPIO_Pin_4
	#define Red_GPIO_Port 		GPIOA
	#define Red_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)
	
	void Red_ON(void);
	void Red_OFF(void);
	void Red_Turn(void);
#endif
#if defined(LED_GREEN)
	#define Green_GPIO_Pin 		GPIO_Pin_8
	#define Green_GPIO_Port 	GPIOC
	#define Green_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

	void Green_ON(void);
	void Green_OFF(void);
	void Green_Turn(void);
#endif
#if defined(LED_YELLOW)
	#define Yellow_GPIO_Pin 	GPIO_Pin_9
	#define Yellow_GPIO_Port 	GPIOC
	#define Yellow_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

	void Yellow_ON(void);
	void Yellow_OFF(void);
	void Yellow_Turn(void);
#endif
#if defined(LED_BLUE)
	#define Blue_GPIO_Pin 		GPIO_Pin_13
	#define Blue_GPIO_Port 		GPIOC
//	#define Blue_GPIO_Pin 		GPIO_Pin_15
//	#define Blue_GPIO_Port 		GPIOA
	#define Blue_GPIO_CLK_ENABLE()          	do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

	void Blue_ON(void);
	void Blue_OFF(void);
	void Blue_Turn(void);
#endif
#if defined(LED_WHITE)
	#define White_GPIO_Pin 	GPIO_Pin_3
	#define White_GPIO_Port 	GPIOC
	#define White_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); }while(0)

	void White_ON(void);
	void White_OFF(void);
	void White_Turn(void);
#endif
	
void LED_Init(void);

//	void Red_ON(void);
//	void Red_OFF(void);
//	void Red_Turn(void);

//	void Green_ON(void);
//	void Green_OFF(void);
//	void Green_Turn(void);

//	void Yellow_ON(void);
//	void Yellow_OFF(void);
//	void Yellow_Turn(void);

//	void Blue_ON(void);
//	void Blue_OFF(void);
//	void Blue_Turn(void);

//	void White_ON(void);
//	void White_OFF(void);
//	void White_Turn(void);

#endif
