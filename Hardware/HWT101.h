#ifndef __HWT101_H
#define __HWT101_H

#define HWT101_ADDR 0x50
int16_t HWT101_ReadYawRaw(void);

float HWT101_CalcYawAngle(int16_t yawRaw);
float HWT101_ReadYaw(void);

uint8_t HWT101_CaliYaw(void);

uint8_t HWT101_Init(void);

#endif
