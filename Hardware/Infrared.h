#ifndef ___Infraed_H
#define ___Infraed_H

#include "stm32f10x.h"                  // Device header


// NECЭ�鳣��(˫�߲���)
#define NEC_HDR_MARK   9000    // ������ߵ�ƽʱ��(us)
#define NEC_HDR_SPACE  4500    // ������͵�ƽʱ��(us)
#define NEC_BIT_MARK   560     // Ϊ�ߵ�ƽʱ��(us)
#define NEC_ONE_SPACE  1690    // �߼�1�͵�ƽʱ��(us)
#define NEC_ZERO_SPACE 560     // �߼�0�͵�ƽʱ��(us)
#define NEC_RPT_SPACE  2250    // �ظ���͵�ƽʱ��(us)


// NECЭ�鳣��(�½��ز���ģʽ)��ͨ�������½��صļ��ʱ���ж��źţ���+����ʱ����
#define NEC_HDR_PULSE   13500   // ��������ʱ��(us)��9000���ߣ�+4500���ͣ�=13500us
#define NEC_ONE_PULSE   2250    // �߼�1��ʱ��(us)��560���ߣ�+1690���ͣ�=2250us
#define NEC_ZERO_PULSE  1120    // �߼�0��ʱ��(us)��560���ߣ�+560���ͣ�=1120us
#define NEC_RPT_PULSE   11750   // �ظ�����ʱ��(us)��9000���ߣ�+2250���ͣ�=11250us���˴��궨�����Ϊʵ��΢��ֵ


// ȫ�ֱ���
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
