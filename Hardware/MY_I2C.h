#ifndef __MY_I2C_H
#define __MY_I2C_H

void MyI2C_SCL_WriteBit(uint8_t Bit);
void MyI2C_SDA_WriteBit(uint8_t Bit);
uint8_t MyI2C_SDA_ReadBit(void);
void My_I2CInit(void);
void My_I2CStart(void);
void My_I2CStop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t Ack);
uint8_t MyI2C_ReceiveAck(void);


#endif

