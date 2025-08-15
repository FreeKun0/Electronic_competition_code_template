#ifndef __HUIDU8_H
#define __HUIDU8_H

#include "stm32f10x.h"
#include "DELAY.H"

void GW_Gray_ReadDigitalData(void);
void huidu8_Init(void);
extern uint8_t gray[8];

#endif
