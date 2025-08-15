#ifndef ___Infraed_H
#define ___Infraed_H

#include "stm32f10x.h"                  // Device header


// NEC协议常量(双边捕获)
#define NEC_HDR_MARK   9000    // 引导码高电平时间(us)
#define NEC_HDR_SPACE  4500    // 引导码低电平时间(us)
#define NEC_BIT_MARK   560     // 为高电平时间(us)
#define NEC_ONE_SPACE  1690    // 逻辑1低电平时间(us)
#define NEC_ZERO_SPACE 560     // 逻辑0低电平时间(us)
#define NEC_RPT_SPACE  2250    // 重复码低电平时间(us)


// NEC协议常量(下降沿捕获模式)：通过两次下降沿的间隔时间判断信号（高+低总时长）
#define NEC_HDR_PULSE   13500   // 引导码总时长(us)：9000（高）+4500（低）=13500us
#define NEC_ONE_PULSE   2250    // 逻辑1总时长(us)：560（高）+1690（低）=2250us
#define NEC_ZERO_PULSE  1120    // 逻辑0总时长(us)：560（高）+560（低）=1120us
#define NEC_RPT_PULSE   11750   // 重复码总时长(us)：9000（高）+2250（低）=11250us（此处宏定义可能为实测微调值


// 全局变量
extern	uint8_t  IR_Data[4];     
extern	uint8_t  IR_Addr;        
extern	uint8_t  IR_AddrRev;     
extern	uint8_t  IR_Cmd;         
extern	uint8_t  IR_CmdRev;      
extern	uint8_t  IR_DataReady;   
extern	uint8_t  IR_Repeat;      
extern	uint32_t IR_PreTime;     
extern	uint8_t  IR_State;       
extern	uint8_t  IR_BitCount;    	

void TIM1_Init(uint32_t PSC,uint32_t ARR);
void Infrared_Init(void);
void IR_Decode(uint32_t time);
void IR_Falling_Decode(uint32_t time);
uint8_t IR_GetNum(void);

#endif 
