#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"                  // Device header
//#include "parameter.h"


extern uint8_t Load_flag;
extern float AD_Voltage;

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
float AD_GetVoltage(uint8_t ADC_Channel);
void LoadOrNot(void);

#endif
