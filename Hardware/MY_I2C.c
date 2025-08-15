#include "stm32f10x.h"                  
#include "DELAY.H"

// IO重定义
#define SCL_PORT    GPIOB
#define SCL_PIN     GPIO_Pin_0
#define SDA_PORT    GPIOB
#define SDA_PIN     GPIO_Pin_1

#define SCL_HIGH    GPIO_SetBits(SCL_PORT, SCL_PIN)
#define SCL_LOW     GPIO_ResetBits(SCL_PORT, SCL_PIN)
#define SDA_HIGH    GPIO_SetBits(SDA_PORT, SDA_PIN)
#define SDA_LOW     GPIO_ResetBits(SDA_PORT, SDA_PIN)
#define SDA_READ    GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)

void MyI2C_SCL_WriteBit(uint8_t Bit)
{
    if(Bit) SCL_HIGH;
    else SCL_LOW;
    Delay_us(10);
}

void MyI2C_SDA_WriteBit(uint8_t Bit)
{
    if(Bit) SDA_HIGH;
    else SDA_LOW;
    Delay_us(10);
}

uint8_t MyI2C_SDA_ReadBit(void)
{
    return SDA_READ;
}

void My_I2CInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SCL_PORT, &GPIO_InitStructure);
    
    SCL_HIGH;
    SDA_HIGH;
}

void My_I2CStart(void)
{
    MyI2C_SDA_WriteBit(1);
    MyI2C_SCL_WriteBit(1);
    MyI2C_SDA_WriteBit(0);
    MyI2C_SCL_WriteBit(0);
}

void My_I2CStop(void)
{
    MyI2C_SDA_WriteBit(0);
    MyI2C_SCL_WriteBit(1);
    MyI2C_SDA_WriteBit(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        MyI2C_SDA_WriteBit(Byte & (0x80 >> i));
        MyI2C_SCL_WriteBit(1);
        MyI2C_SCL_WriteBit(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_SDA_WriteBit(1);
    for(i = 0; i < 8; i++)
    {
        MyI2C_SCL_WriteBit(1);
        Byte |= MyI2C_SDA_ReadBit() << (7 - i);
        MyI2C_SCL_WriteBit(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t Ack)
{
    MyI2C_SDA_WriteBit(Ack);
    MyI2C_SCL_WriteBit(1);
    MyI2C_SCL_WriteBit(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t Ack;
    MyI2C_SDA_WriteBit(1);
    MyI2C_SCL_WriteBit(1);
    Ack = MyI2C_SDA_ReadBit();
    MyI2C_SCL_WriteBit(0);
    return Ack;
}
