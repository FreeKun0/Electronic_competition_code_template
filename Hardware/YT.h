#ifndef __YT_H
#define __YT_H

#include "stm32f10x.h"
#include "Delay.h"
#include "bsp_pid.h"
#include <math.h>

#include "Serial.h"
#include "k210.h"
#include "UART4.h"
#include "UART5.h"

#include "Timer.h"

#include "OLED.h"
#include "LED.h"
#include "Beep.h"
#include "Key.h"
#include "Keyboard.h"
#include "gray.h"
#include "AD.h"


#include "control.h"
#include "Servo.h"
#include "Motor.h"
#include "Encoder.h"
#include "Infrared.h"
#include "mpu6050.h"
#include "parameter.h"
#include "bsp_pid.h"
#include "HWT101.h"
extern uint8_t Do_count1;
extern uint8_t Do_count2;
extern uint8_t Do_count3;
extern uint8_t Do_count4;
extern uint8_t Do_count5;
extern uint8_t Do_count6;
extern uint8_t Do_count7;
extern uint8_t Do_count8;
extern uint8_t Do_count9;
extern uint8_t Do_count10;
extern uint8_t Do_count11;
extern uint8_t Do_count12;

extern uint8_t KeyNum;			//���հ�������
extern uint8_t KeyBoardNum;	//���վ��󰴼�����
extern uint8_t IR_Num;			//���պ���ң�ؼ���
extern uint8_t Usart2Num;

#define WATING_PENCIL_LINE_10MS		(1000 * 1)		//1000ms�ƶ�һ����
#define CHANGE_PWM	100

#define PWM_STEP_SMALL	3
#define PWM_STEP_LARGE	20

#include <math.h>
#include <stdarg.h>
//#define MY_PI 3.1415926535f
#define MAX_POINTS 1000
#define MAX_VERTICES 20  // ��󶥵���

// ͼ������ö��
typedef enum {
	SHAPE_LINE,             // ֱ��
	SHAPE_TRIANGLE,         // ������
	SHAPE_ROT_RECT,         // ��б����
	SHAPE_CIRCLE,           // Բ��
	SHAPE_POLYGON,          // ��������
	SHAPE_REGULAR_POLYGON   // ������Σ�֧����б�Ƕȣ�
} ShapeType;

// ֱ�߲����ṹ��
typedef struct {
	float x0, y0;           // �������
	float x1, y1;           // �յ�����
	uint16_t segments;      // �ֶ���
} LineParams;

// �����β����ṹ��
typedef struct {
	float p1x, p1y;         // ����1����
	float p2x, p2y;         // ����2����
	float p3x, p3y;         // ����3����
	uint16_t segments_per_side;  // ÿ�߷ֶ���
} TriangleParams;

// ��б���β����ṹ��
typedef struct {
	float x0, y0;           // ���Ͻ�����
	float width;            // ���
	float height;           // �߶�
	float angle;            // ��б�Ƕȣ��ȣ�
	uint16_t segments_per_side;  // ÿ�߷ֶ���
} RotRectParams;

// Բ�β����ṹ��
typedef struct {
	float center_x;         // Բ��X����
	float center_y;         // Բ��Y����
	float radius;           // �뾶
	uint16_t segments;      // �ֶ�����Խ��ԽԲ��
} CircleParams;

// �������β����ṹ��
typedef struct {
	float vertices[MAX_VERTICES][2];  // �������� [i][0]=x, [i][1]=y
	uint8_t vertex_count;     // ������������3��
	uint16_t segments_per_side;  // ÿ�߷ֶ���
} PolygonParams;

// ������β����ṹ�壨֧����б�Ƕȣ�
typedef struct {
	float center_x;          // ����X����
	float center_y;          // ����Y����
	float radius;            // ���Բ�뾶
	uint8_t sides;           // ��������3��
	float angle_deg;         // ��б�Ƕȣ��ȣ�
	uint16_t segments_per_side;  // ÿ�߷ֶ���
} RegularPolygonParams;

// ͨ��ͼ��״̬�ṹ��
typedef struct {
	uint8_t run_flag;        // ���б�־��1=���У�0=ֹͣ��
	uint8_t init_flag;       // ��ʼ����־��1=�ѳ�ʼ����
	uint16_t point_idx;      // ��ǰ������
	uint16_t total_points;   // �ܵ���
	uint8_t period;          // ÿ�����ͣ������
	ShapeType curr_shape;    // ��ǰͼ������

	union {
		LineParams line;             // ֱ�߲���
		TriangleParams tri;          // �����β���
		RotRectParams rect;          // ���β���
		CircleParams circle;         // Բ�β���
		PolygonParams polygon;       // �������β���
		RegularPolygonParams regular_poly;  // ������β���
	} params;

} ShapeState;

// ȫ�ֹ��õ�����
extern float shared_x_points[MAX_POINTS];
extern float shared_y_points[MAX_POINTS];

// ȫ��״̬����
extern ShapeState shape_state;

// ��������
void GenerateLinePoints(float x1, float y1, float x2, float y2, uint16_t segments, 
					   float x_array[], float y_array[], uint16_t* start_idx);
void GeneratePolygonPoints(PolygonParams* poly, float x_array[], float y_array[], uint16_t* total_points);
void GenerateRegularPolygonVertices(RegularPolygonParams* regular_poly, float vertices[][2], uint8_t* vertex_count);
void GenerateShapePoints(void);

// ֱ�߳�ʼ������
void Line_Init(float x0, float y0, float x1, float y1, uint16_t segments, uint16_t period);
// �����γ�ʼ������
void Triangle_Init(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, 
                  uint16_t segments_per_side, uint16_t period);
// ��б���γ�ʼ������
void RotRect_Init(float x0, float y0, float width, float height, float angle_deg, 
                 uint16_t segments_per_side, uint16_t period);
// Բ�γ�ʼ������
void Circle_Init(float center_x, float center_y, float radius, uint16_t segments, uint16_t period);
// �������γ�ʼ������
void Polygon_Init(float(*vertices)[2], uint8_t vertex_count, uint16_t segments_per_side, uint16_t period);
// ������γ�ʼ������
void RegularPolygon_Init(float center_x, float center_y, float radius, uint8_t sides, 
                        float angle_deg, uint16_t segments_per_side, uint16_t period);
void Shape_Control(void);









	
void pencil_piont_get_1(void);
void pencil_piont_get_2(void);
void pencil_piont_get_3(void);
void red_piont_reset_1(void);
void red_piont_reset_2(void);
void pencil_line_1(void);		//ѲǦ���� ���� 5PWM ֱ��
void pencil_line_2(void);		//ѲǦ���� �ջ� pid 
void pencil_line_3(void);		//ѲǦ���� �ջ� ���� �յ㷴�� 5����
void pencil_line_4(void);		//ѲǦ���� �ջ� ���� �յ㷴�� 5PWM
void pencil_line_5(void);		//ѲǦ���� ���� 5PWM ����
void black_piont_get_guding_1(void);	//��Ŀ�� ��ȡ�̶��ڿ�PWM
void black_line_guding_1(void);	//��Ŀ�� �߹̶��ڿ�
void black_piont_get_suiji_1(void);
void black_line_1(void);
void reset_zhuizong_1(void);
void yundong_zhuizong_1(void);
void tar_cur_pionts(void);

extern uint16_t pencil_points_PWM[5][2];
extern uint16_t black_points_PWM_guding[4][2];
extern uint16_t PWM1_for_TASK1;
extern uint16_t PWM2_for_TASK1;
extern uint16_t PWM_step_for_TASK1;	//����10PWM��KEY3�л�Ϊ1PWM

#endif
