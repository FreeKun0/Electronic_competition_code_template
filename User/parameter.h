#ifndef __PARAMETER_H
#define __PARAMETER_H
#include "stm32f10x.h"                  // Device header

/* Բ���� */
#define MY_PI	3.14159265359

void reset(void);
float Limit_uint32_t(uint32_t num, uint32_t upper, uint32_t lower);
float Limit_int32_t(int32_t num, int32_t upper, int32_t lower);
float Limit_uint16_t(uint16_t num, uint16_t upper, uint16_t lower);
float Limit_int16_t(int16_t num, int16_t upper, int16_t lower);
float Limit_float(float num, float upper, float lower);

/* �����ṹ���ʼ�� */
typedef struct
{
	uint8_t uart_rx[50];
	uint8_t HMI_rx[50];
	
	uint16_t beep_ms;
	uint16_t red_ms;
	uint16_t green_ms;
	uint16_t yellow_ms;
	uint16_t blue_ms;
	uint16_t white_ms;
	
	/* ������� */
	
	uint16_t stepx_run_ms;
	uint16_t stepx_stop_ms;
	uint32_t stepx_speed;
	uint8_t stepx_dir;
	
	
	/* ��̨ */
	float pid_x_Outval;
	float pid_y_Outval;
	
	float pid2_x_Outval;
	float pid2_y_Outval;
	
	
	/* С�� */
	float Target_Journey_cm;					//Car_go�����õ�Ŀ����룬��λcm�������ж�С���쵽Ŀ����봦��Ҫ����һ�׶�����������
	float Motor1_journey_cm, Motor2_journey_cm;	//�����ʻ���룬��λcm
	float Motor1_PWM, Motor2_PWM;				//���������PWM����λPWM
	float speed1_Outval, speed2_Outval;			//�ٶȻ������PWM����λPWM
	float location1_Outval, location2_Outval;	//λ�û������Ŀ���ٶȣ���λRPM�������ٶȻ�	
	float yaw_Outval;
	
}Param_InitTypeDef;

/* ��־λ�ṹ���ʼ�� */
typedef struct
{
	uint8_t _flag;
	uint8_t beep_flag;		//��������ʱ
	uint8_t red_flag;
	uint8_t green_flag;
	uint8_t yellow_flag;
	uint8_t blue_flag;
	uint8_t white_flag;
	
	uint8_t counting_flag;	//��ʱ
	
	/* ��̨ */
	
	uint8_t reset_flag;
	uint8_t stop_flag;
	uint8_t reach_flag;
	
	uint8_t wating_pencil_line_flag;
	
	uint8_t pencil_line_count_increase_flag;
	
	//��ȡǦ�ʽǵ�
	uint8_t get_one_pencil_succeed_flag;
	//��Ǧ����
	uint8_t pencil_line_flag;
	uint8_t pencil_line_succeed_flag;
	//��ȡ���νǵ�
	uint8_t black_pionts_get_flag;
	uint8_t black_path_pionts_get_succeed_flag;
	//�߾��ο�
	uint8_t black_line_flag;
	uint8_t black_line_succeed_flag;
	//��λ׷��
	uint8_t green_reset_flag;
	uint8_t green_reset_succeed_flag;
	//�˶�׷��
	uint8_t zhuizong_flag;
	uint8_t zhuizong_succeed_flag;
	//Ŀ��ֵʵ��ֵ
	uint8_t cur_catch_tar_flag;
	uint8_t tar_cur_flag;
	uint8_t tar_cur_succeed_flag;
	
	/* С�� */
	uint8_t YAW_succeed_flag;			//YAWУ׼�ɹ�
	uint8_t find_satrt_flag;			//K210ʶ��
	uint8_t spin_start_flag;			//����ʼת��
	uint8_t spin_succeed_flag;			//�ɹ�ת��
//	uint8_t stop_flag;					//ͣ
	uint8_t TWO_wait_flag;				//�����ȴ�
	uint8_t line_flag;					//ֱ�У�Ѳ�ߣ�
	uint8_t back_180_flag_cm;			//��ת���벹��
	uint8_t left_90_flag_angle;			//��ת�ǶȲ���
	uint8_t right_90_flag_angle;		//��ת�ǶȲ���
	uint8_t back_180_flag_angle;		//��ת�ǶȲ���
	uint8_t blue_load_flag;				//����ģ��װҩ��
	uint8_t move_at_any_speed_flag;		//�������ٶ��ƶ�

	
}Flag_InitTypeDef;

/* �����ṹ���ʼ�� */
typedef struct
{
	uint8_t count;
	uint16_t _count;
	
	
	uint16_t beep_count;	//��������ʱ
	uint16_t red_count;		//��������ʱ
	uint16_t green_count;
	uint16_t yellow_count;
	uint16_t blue_count;
	uint16_t white_count;
	
	uint32_t counting_time;	//��ʱ
	
	/* ������� */
	uint16_t stepx_count;
	
	/* ��̨ */
	uint8_t pencil_count;	//��¼���������
	uint8_t black_count_3;	//��¼�ڿ�PWM�� ������

	uint16_t wating_pencil_line_count;	//Ǧ���߶����ƶ� �ȴ������ת��λ
	uint32_t pencil_line_count;		//Ǧ���ߣ��յ㷴���� ��������ȡ��
	uint8_t current_pencil_count;
	
	uint16_t black_spend_time_count;
	
	/* С�� */
	uint8_t load_cont;					//���ҩ�����
	uint8_t BlueTooth_send_time_count;	//���ͽ��ģ�K210/˫������
	uint8_t OLED_Show_cont;				//OLED��ʾ����
	uint16_t find_time_count;			//K210ʶ���ʱ
	uint16_t stop_count, spin_count;	//ɲ��ĩβ��ʱ��ת���ʱ
	uint16_t turn_bucahng_count;		//��ת��������
	uint16_t TWO_wait_time_count;		//�����ȴ���ʱ
	
	
}Count_InitTypeDef;

extern Param_InitTypeDef param;
extern Flag_InitTypeDef  flag;
extern Count_InitTypeDef count;

#endif


