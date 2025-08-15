#include "stm32f10x.h"
#include "DELAY.H"
#include "MY_I2C.h"  // 包含自定义I2C驱动
#include "HWT101.h"
#include "OLED.h"
// HWT101默认I2C地址（7位地址，读写时需左移1位并添加读写位）
#define HWT101_ADDR 0x50

uint8_t HWT101_Init(void)
{
	My_I2CInit();
	return HWT101_CaliYaw();
}

/**
 * @brief  从HWT101读取偏航角（Yaw）的原始数据
 * @return int16_t 原始Yaw数据（16位有符号整数，范围-32768~32767，对应角度-180°~180°）
 */
int16_t HWT101_ReadYawRaw(void) {
    uint8_t yawH, yawL;  // 存储Yaw角的高8位和低8位数据
    int16_t yawRaw;      // 合并后的原始Yaw数据

    // 1. 发送I2C起始信号
    My_I2CStart();

    // 2. 发送HWT101的写地址（7位地址左移1位，最低位0表示写操作）
    MyI2C_SendByte(HWT101_ADDR << 1);
    // 等待HWT101的应答信号（0表示应答成功）
    if (MyI2C_ReceiveAck() != 0) {
        My_I2CStop();  // 若未应答，发送停止信号
        return 0;      // 读取失败，返回0
    }

    // 3. 发送Yaw角数据的寄存器地址（此处使用0x3F，根据实际测试调整）
    MyI2C_SendByte(0x3F);  // 发送要读取的寄存器地址
    if (MyI2C_ReceiveAck() != 0) {  // 等待应答
        My_I2CStop();
        return 0;
    }

    // 4. 发送重复起始信号（切换到读操作，避免总线释放）
    My_I2CStart();

    // 5. 发送HWT101的读地址（7位地址左移1位，最低位1表示读操作）
    MyI2C_SendByte(HWT101_ADDR << 1 | 1);
    if (MyI2C_ReceiveAck() != 0) {  // 等待应答
        My_I2CStop();
        return 0;
    }

    // 6. 读取Yaw角的低8位数据（先读低8位，根据实际协议调整顺序）
    yawL = MyI2C_ReceiveByte();
    MyI2C_SendAck(0);  // 发送应答信号（0表示继续读取）

    // 7. 读取Yaw角的高8位数据（再读高8位）
    yawH = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);  // 发送非应答信号（1表示停止读取）

    // 8. 发送I2C停止信号，结束通信
    My_I2CStop();

    // 9. 合并高8位和低8位数据，得到16位原始值
    yawRaw = (int16_t)((yawH << 8) | yawL);
    return yawRaw;
}

/**
 * @brief  将原始Yaw数据转换为角度值
 * @param  yawRaw 原始Yaw数据（HWT101_ReadYawRaw的返回值）
 * @return float  转换后的角度值（单位：度，范围-180.0°~180.0°）
 */
float HWT101_CalcYawAngle(int16_t yawRaw) {
    // 转换公式：角度 = 原始值 ÷ 32768 × 180（根据HWT101手册默认比例）
    return (float)yawRaw / 32768.0f * 180.0f;
}

/**
 * @brief  读取HWT101的偏航角（Yaw）角度值
 * @return float 偏航角角度（单位：度）
 */
float HWT101_ReadYaw(void) {
    int16_t rawData = HWT101_ReadYawRaw();  // 获取原始数据
    return HWT101_CalcYawAngle(rawData);    // 转换为角度值
}



// 解锁寄存器指令
const uint8_t UNLOCK_CMD[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
// Z轴角度归零指令
const uint8_t CALIYAW_CMD[5] = {0xFF, 0xAA, 0x76, 0x00, 0x00};
// 保存设置指令
const uint8_t SAVE_CMD[5] = {0xFF, 0xAA, 0x00, 0x00, 0x00};

/**
 * @brief  向HWT101发送多个字节的数据
 * @param  data 要发送的数据缓冲区
 * @param  len  数据长度
 * @return uint8_t 0表示成功，非0表示失败
 */
uint8_t MyI2C_SendMultiBytes(const uint8_t *data, uint8_t len) {
    uint8_t i;

    // 发送I2C起始信号
    My_I2CStart();

    // 发送HWT101的写地址
    MyI2C_SendByte(HWT101_ADDR << 1);
    if (MyI2C_ReceiveAck() != 0) {
        My_I2CStop();
        return 1; // 失败
    }

    // 发送多个字节数据
    for (i = 0; i < len; i++) {
        MyI2C_SendByte(data[i]);
        if (MyI2C_ReceiveAck() != 0) {
            My_I2CStop();
            return 1; // 失败
        }
    }

    // 发送I2C停止信号
    My_I2CStop();
    return 0; // 成功
}

/**
 * @brief  CALIYAW（Z轴角度归零）函数
 * @return uint8_t 0表示成功，非0表示失败
 */
uint8_t HWT101_CaliYaw(void) {
    // 1. 解锁寄存器
    if (MyI2C_SendMultiBytes(UNLOCK_CMD, 5) != 0) {
        return 1; // 解锁失败
    }
    Delay_ms(10); // 延时处理

    // 2. 发送Z轴角度归零指令
    if (MyI2C_SendMultiBytes(CALIYAW_CMD, 5) != 0) {
        return 2; // 发送归零指令失败
    }
    Delay_ms(10); // 延时处理

    // 3. 保存设置
    if (MyI2C_SendMultiBytes(SAVE_CMD, 5) != 0) {
        return 3; // 保存设置失败
    }
    Delay_ms(10); // 延时处理

    return 0; // 成功
}

