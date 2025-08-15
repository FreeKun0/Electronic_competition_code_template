#include "huidu8.h"

/*_______________________iic部分_________________________________________________*/
/*_______________________iic部分_________________________________________________*/

// IO重定义
#define huidu_RCC_SCL     RCC_APB2Periph_GPIOC
#define huidu_RCC_SDA     RCC_APB2Periph_GPIOC
#define huidu_SCL_PORT    GPIOC
#define huidu_SCL_PIN     GPIO_Pin_0
#define huidu_SDA_PORT    GPIOC
#define huidu_SDA_PIN     GPIO_Pin_1

#define huidu_SCL_HIGH    GPIO_SetBits(huidu_SCL_PORT, huidu_SCL_PIN)
#define huidu_SCL_LOW     GPIO_ResetBits(huidu_SCL_PORT, huidu_SCL_PIN)
#define huidu_SDA_HIGH    GPIO_SetBits(huidu_SDA_PORT, huidu_SDA_PIN)
#define huidu_SDA_LOW     GPIO_ResetBits(huidu_SDA_PORT, huidu_SDA_PIN)
#define huidu_SDA_READ    GPIO_ReadInputDataBit(huidu_SDA_PORT, huidu_SDA_PIN)

void huidu_MyI2C_SCL_WriteBit(uint8_t Bit)
{
    if(Bit) huidu_SCL_HIGH;
    else huidu_SCL_LOW;
    Delay_us(10);
}

void huidu_MyI2C_SDA_WriteBit(uint8_t Bit)
{
    if(Bit) huidu_SDA_HIGH;
    else huidu_SDA_LOW;
    Delay_us(10);
}

uint8_t huidu_MyI2C_SDA_ReadBit(void)
{
    return huidu_SDA_READ;
}


void huidu_My_I2CStart(void)
{
    huidu_MyI2C_SDA_WriteBit(1);
    huidu_MyI2C_SCL_WriteBit(1);
    huidu_MyI2C_SDA_WriteBit(0);
    huidu_MyI2C_SCL_WriteBit(0);
}

void huidu_My_I2CStop(void)
{
    huidu_MyI2C_SDA_WriteBit(0);
    huidu_MyI2C_SCL_WriteBit(1);
    huidu_MyI2C_SDA_WriteBit(1);
}

void huidu_MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        huidu_MyI2C_SDA_WriteBit(Byte & (0x80 >> i));
        huidu_MyI2C_SCL_WriteBit(1);
        huidu_MyI2C_SCL_WriteBit(0);
    }
}

uint8_t huidu_MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    huidu_MyI2C_SDA_WriteBit(1);
    for(i = 0; i < 8; i++)
    {
        huidu_MyI2C_SCL_WriteBit(1);
        Byte |= huidu_MyI2C_SDA_ReadBit() << (7 - i);
        huidu_MyI2C_SCL_WriteBit(0);
    }
    return Byte;
}

void huidu_MyI2C_SendAck(uint8_t Ack)
{
    huidu_MyI2C_SDA_WriteBit(Ack);
    huidu_MyI2C_SCL_WriteBit(1);
    huidu_MyI2C_SCL_WriteBit(0);
}

uint8_t huidu_MyI2C_ReceiveAck(void)
{
    uint8_t Ack;
    huidu_MyI2C_SDA_WriteBit(1);
    huidu_MyI2C_SCL_WriteBit(1);
    Ack = huidu_MyI2C_SDA_ReadBit();
    huidu_MyI2C_SCL_WriteBit(0);
    return Ack;
}

/*灰度初始化函数*/
void huidu8_Init(void)
{
    RCC_APB2PeriphClockCmd(huidu_RCC_SCL, ENABLE);
    RCC_APB2PeriphClockCmd(huidu_RCC_SDA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = huidu_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(huidu_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = huidu_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(huidu_SDA_PORT, &GPIO_InitStructure);
    
    huidu_SCL_HIGH;
    huidu_SDA_HIGH;
}
/*_____________________灰度函数_______________________________________________*/
// 灰度传感器I²C地址（设备地址1001110x，去除读写位后为0x4E）
#define GRAY_ADDR 0x4C  
uint8_t gray[8];

void GW_Gray_ReadDigitalData(void)
{
    uint8_t dev_addr = 0x4C;  // 7位设备地址
    uint8_t command = 0xDD;   // 数字数据命令符
    
    huidu_My_I2CStart();                          // 起始条件
    huidu_MyI2C_SendByte((dev_addr << 1) | 0x00); // 发送设备地址+写标志
    if(huidu_MyI2C_ReceiveAck()) {                // 检查ACK
        huidu_My_I2CStop();
        
    }
    
		huidu_MyI2C_SendByte(command);                // 发送命令符
    if(huidu_MyI2C_ReceiveAck()) {                // 检查ACK
        huidu_My_I2CStop();
       
    }
    
    huidu_My_I2CStart();                          // 重复起始条件
    huidu_MyI2C_SendByte((dev_addr << 1) | 0x01); // 发送设备地址+读标志
    if(huidu_MyI2C_ReceiveAck()) {                // 检查ACK
        huidu_My_I2CStop();
        
    }
    
    uint8_t data = huidu_MyI2C_ReceiveByte();     // 读取数据
    huidu_MyI2C_SendAck(0x01);                    // 发送NACK
    huidu_My_I2CStop();                           // 停止条件
    
    for (int qw = 0; qw < 8; qw++) {
        gray[qw] = (data >> qw) & 0x01; // 提取第i位
			
				if(gray[qw]==1)
					gray[qw]=0;
				else if(gray[qw]==0)
					gray[qw]=1;
    }
}
