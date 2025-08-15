#include "YT.h"
#include <math.h>


uint16_t pencil_points_PWM[5][2] =
{	1660, 1300,		//����
	1355, 1300,		//����
	1355, 1594,		//����
	1655, 1599,		//����
	1506, 1450		//ԭ��
};
uint16_t black_points_PWM_guding[4][2] =
{	1595, 1385,		//����
	1415, 1385,		//����
	1415, 1512,		//����
	1595, 1512,		//����
};
uint16_t PWM1_for_TASK1 = 1500;
uint16_t PWM2_for_TASK1 = 1500;
uint16_t PWM_step_for_TASK1 = PWM_STEP_LARGE;	//����10PWM��KEY3�л�Ϊ1PWM

#include "YT.h"
#include "laser.h"  // ������ƺ�����Laser_P_ON/Laser_P_OFF��

// ȫ�ֹ��õ������ʼ��
float shared_x_points[MAX_POINTS] = {0};
float shared_y_points[MAX_POINTS] = {0};

// ȫ��״̬������ʼ��
ShapeState shape_state = {0};

// ֱ�ߵ����ɺ�������������֮��Ĳ�ֵ��
void GenerateLinePoints(float x1, float y1, float x2, float y2, uint16_t segments, 
					   float x_array[], float y_array[], uint16_t* start_idx)
{
	uint16_t idx = *start_idx;
	float x_step = (x2 - x1) / segments;  // X���򲽳�
	float y_step = (y2 - y1) / segments;  // Y���򲽳�

	// ����ֱ���ϵĵ㣨���������յ㣩
	for (uint16_t i = 0; i <= segments; i++)
	{
		if (idx >= MAX_POINTS)
			break;  // ��ֹ����Խ��
		x_array[idx] = x1 + x_step * i;
		y_array[idx] = y1 + y_step * i;
		idx++;
	}

	*start_idx = idx;  // ������ʼ����������һ����ʹ�ã�
}

// �������ε����ɺ�����������˳�����ӳɱպ�ͼ��
void GeneratePolygonPoints(PolygonParams* poly, float x_array[], float y_array[], uint16_t* total_points)
{
	uint16_t start_idx = 0;
	uint8_t v_count = poly->vertex_count;

	// ������Ҫ3������
	if (v_count < 3)
	{
		*total_points = 0;
		return;
	}

	// ��������ÿ�����㣨v0��v1, v1��v2, ..., vn-1��v0��
	for (uint8_t i = 0; i < v_count; i++)
	{
		// ��ǰ�������һ�����㣨���һ���������ӻص�һ����
		uint8_t next_i = (i + 1) % v_count;
		float x1 = poly->vertices[i][0];
		float y1 = poly->vertices[i][1];
		float x2 = poly->vertices[next_i][0];
		float y2 = poly->vertices[next_i][1];

		// ���ɵ�ǰ�ߵĵ㣨��һ���߱�����㣬�����������ظ��㣩
		uint16_t segments = poly->segments_per_side;
		if (i > 0)
			segments--;  // �����ظ�����

		GenerateLinePoints(x1, y1, x2, y2, segments, x_array, y_array, &start_idx);
	}

	*total_points = start_idx;
}

// ������ζ�����㺯�����������ġ��뾶����������б�Ƕ����ɶ���
void GenerateRegularPolygonVertices(RegularPolygonParams* regular_poly, float vertices[][2], uint8_t* vertex_count)
{
	*vertex_count = 0;
	uint8_t sides = regular_poly->sides;
	if (sides < 3)
		return;  // ��������Ϊ3

	float center_x = regular_poly->center_x;
	float center_y = regular_poly->center_y;
	float radius = regular_poly->radius;
	float angle_rad = regular_poly->angle_deg * MY_PI / 180.0f;  // ��б�Ƕȣ����ȣ�
	float step_angle = 2.0f * MY_PI / sides;  // ÿ������ĽǶȼ��

	// ����ÿ�����������
	for (uint8_t i = 0; i < sides; i++)
	{
		float curr_angle = angle_rad + step_angle * i;  // ��ǰ����ĽǶȣ�����б��
		vertices[i][0] = center_x + radius * cos(curr_angle);
		vertices[i][1] = center_y + radius * sin(curr_angle);
		(*vertex_count)++;
	}
}

// ����ͼ�����е���ܺ���
void GenerateShapePoints(void)
{
	uint16_t start_idx = 0;
	shape_state.total_points = 0;

	switch (shape_state.curr_shape)
	{
		case SHAPE_LINE:
		{
			// ����ֱ�ߵ�
			GenerateLinePoints(
				shape_state.params.line.x0, shape_state.params.line.y0,
				shape_state.params.line.x1, shape_state.params.line.y1,
				shape_state.params.line.segments,
				shared_x_points, shared_y_points, &start_idx
			);
			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_TRIANGLE:
		{
			uint16_t seg = shape_state.params.tri.segments_per_side;

			// ��һ���ߣ�p1��p2
			GenerateLinePoints(
				shape_state.params.tri.p1x, shape_state.params.tri.p1y,
				shape_state.params.tri.p2x, shape_state.params.tri.p2y,
				seg, shared_x_points, shared_y_points, &start_idx
			);

			// �ڶ����ߣ�p2��p3�������ظ��㣩
			GenerateLinePoints(
				shape_state.params.tri.p2x, shape_state.params.tri.p2y,
				shape_state.params.tri.p3x, shape_state.params.tri.p3y,
				seg - 1, shared_x_points, shared_y_points, &start_idx
			);

			// �������ߣ�p3��p1�������ظ��㣩
			GenerateLinePoints(
				shape_state.params.tri.p3x, shape_state.params.tri.p3y,
				shape_state.params.tri.p1x, shape_state.params.tri.p1y,
				seg - 1, shared_x_points, shared_y_points, &start_idx
			);

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_ROT_RECT:
		{
			uint16_t seg = shape_state.params.rect.segments_per_side;
			float x0 = shape_state.params.rect.x0;
			float y0 = shape_state.params.rect.y0;
			float w = shape_state.params.rect.width;
			float h = shape_state.params.rect.height;
			float angle_rad = shape_state.params.rect.angle * MY_PI / 180.0f;
			float cos_ang = cos(angle_rad);
			float sin_ang = sin(angle_rad);

			// ��������ĸ�����
			float p0x = x0, p0y = y0;
			float p1x = x0 + w * cos_ang, p1y = y0 + w * sin_ang;
			float p2x = p1x - h * sin_ang, p2y = p1y + h * cos_ang;
			float p3x = x0 - h * sin_ang, p3y = y0 + h * cos_ang;

			// �����ߣ�p0��p1��p2��p3��p0
			GenerateLinePoints(p0x, p0y, p1x, p1y, seg, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p1x, p1y, p2x, p2y, seg - 1, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p2x, p2y, p3x, p3y, seg - 1, shared_x_points, shared_y_points, &start_idx);
			GenerateLinePoints(p3x, p3y, p0x, p0y, seg - 1, shared_x_points, shared_y_points, &start_idx);

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_CIRCLE:
		{
			uint16_t segments = shape_state.params.circle.segments;
			float center_x = shape_state.params.circle.center_x;
			float center_y = shape_state.params.circle.center_y;
			float radius = shape_state.params.circle.radius;

			// �ò�����������Բ�ܵ�
			for (uint16_t i = 0; i <= segments; i++)
			{
				if (start_idx >= MAX_POINTS)
					break;
				float angle = 2.0f * MY_PI * i / segments;  // �Ƕȣ����ȣ�
				shared_x_points[start_idx] = center_x + radius * cos(angle);
				shared_y_points[start_idx] = center_y + radius * sin(angle);
				start_idx++;
			}

			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_POLYGON:
		{
			// �����������ε�
			GeneratePolygonPoints(
				&shape_state.params.polygon,
				shared_x_points, shared_y_points, &start_idx
			);
			shape_state.total_points = start_idx;
			break;
		}

		case SHAPE_REGULAR_POLYGON:
		{
			// ����������ζ���
			float vertices[MAX_VERTICES][2];
			uint8_t vertex_count = 0;
			GenerateRegularPolygonVertices(
				&shape_state.params.regular_poly,
				vertices, &vertex_count
			);

			// ת��Ϊ�������β��������ɵ�
			PolygonParams temp_poly;
			temp_poly.vertex_count = vertex_count;
			temp_poly.segments_per_side = shape_state.params.regular_poly.segments_per_side;
			for (uint8_t i = 0; i < vertex_count; i++)
			{
				temp_poly.vertices[i][0] = vertices[i][0];
				temp_poly.vertices[i][1] = vertices[i][1];
			}

			GeneratePolygonPoints(&temp_poly, shared_x_points, shared_y_points, &start_idx);
			shape_state.total_points = start_idx;
			break;
		}
	}
}

// ֱ�߳�ʼ������
void Line_Init(float x0, float y0, float x1, float y1, uint16_t segments, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_LINE;

    // ����ֱ�߲���
    shape_state.params.line.x0 = x0;
    shape_state.params.line.y0 = y0;
    shape_state.params.line.x1 = x1;
    shape_state.params.line.y1 = y1;
    shape_state.params.line.segments = segments;
    shape_state.period = period;
}

// �����γ�ʼ������
void Triangle_Init(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, 
                  uint16_t segments_per_side, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_TRIANGLE;

    // ���������β���
    shape_state.params.tri.p1x = p1x;
    shape_state.params.tri.p1y = p1y;
    shape_state.params.tri.p2x = p2x;
    shape_state.params.tri.p2y = p2y;
    shape_state.params.tri.p3x = p3x;
    shape_state.params.tri.p3y = p3y;
    shape_state.params.tri.segments_per_side = segments_per_side;
    shape_state.period = period;
}

// ��б���γ�ʼ������
void RotRect_Init(float x0, float y0, float width, float height, float angle_deg, 
                 uint16_t segments_per_side, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_ROT_RECT;

    // ���þ��β���
    shape_state.params.rect.x0 = x0;
    shape_state.params.rect.y0 = y0;
    shape_state.params.rect.width = width;
    shape_state.params.rect.height = height;
    shape_state.params.rect.angle = angle_deg;
    shape_state.params.rect.segments_per_side = segments_per_side;
    shape_state.period = period;
}

// Բ�γ�ʼ������
void Circle_Init(float center_x, float center_y, float radius, uint16_t segments, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_CIRCLE;

    // ����Բ�β���
    shape_state.params.circle.center_x = center_x;
    shape_state.params.circle.center_y = center_y;
    shape_state.params.circle.radius = radius;
    shape_state.params.circle.segments = segments;
    shape_state.period = period;
}

// �������γ�ʼ������
void Polygon_Init(float(*vertices)[2], uint8_t vertex_count, uint16_t segments_per_side, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_POLYGON;

    // ���涥�㣨������󶥵�����
    shape_state.params.polygon.vertex_count = (vertex_count <= MAX_VERTICES) ? vertex_count : MAX_VERTICES;
    for (uint8_t i = 0; i < shape_state.params.polygon.vertex_count; i++)
    {
        shape_state.params.polygon.vertices[i][0] = vertices[i][0];
        shape_state.params.polygon.vertices[i][1] = vertices[i][1];
    }
    shape_state.params.polygon.segments_per_side = (segments_per_side > 4) ? segments_per_side : 10;
    shape_state.period = period;
}

// ������γ�ʼ������
void RegularPolygon_Init(float center_x, float center_y, float radius, uint8_t sides, 
                        float angle_deg, uint16_t segments_per_side, uint16_t period)
{
    // ����״̬
    shape_state.run_flag = 1;
    shape_state.init_flag = 0;
    shape_state.point_idx = 0;
    shape_state.curr_shape = SHAPE_REGULAR_POLYGON;

    // ����������β���
    shape_state.params.regular_poly.center_x = center_x;
    shape_state.params.regular_poly.center_y = center_y;
    shape_state.params.regular_poly.radius = radius;
    shape_state.params.regular_poly.sides = sides;
    shape_state.params.regular_poly.angle_deg = angle_deg;
    shape_state.params.regular_poly.segments_per_side = segments_per_side;
    shape_state.period = period;

    // ȷ��������3
    if (shape_state.params.regular_poly.sides < 3)
        shape_state.params.regular_poly.sides = 3;
}

// ͼ�ο��ƺ�������ʱ�ж��е��ã�
void Shape_Control(void)
{
	if (shape_state.run_flag != 1)
		return;

	// �״����У�����ͼ�����е�
	if (shape_state.init_flag == 0)
	{
		GenerateShapePoints();
		shape_state.init_flag = 1;
		shape_state.point_idx = 0;
		Laser_P_OFF();  // �رռ��⣬׼���ƶ������
		return;
	}

	// �׶�1���ƶ�����һ���㣨����رգ��ȴ�50�����ڣ�
	if (shape_state.point_idx == 0)
	{
		static uint16_t move_to_first_cnt = 0;

		// �������͵�һ���������
		extern float rotation_red[2];
		extern void Servo_All_SetPWM(float, float);
		rotation_red[0] = shared_x_points[0];
		rotation_red[1] = shared_y_points[0];
		Servo_All_SetPWM(rotation_red[0], rotation_red[1]);

		// �ȴ�50������ȷ����λ
		if (move_to_first_cnt < 50)
		{
			move_to_first_cnt++;
		}
		else
		{
			move_to_first_cnt = 0;
			Laser_P_ON();  // ������㣬��������
			shape_state.point_idx++;
		}
		return;
	}

	// �׶�2���������ƣ����⿪����
	if (shape_state.point_idx < shape_state.total_points && shape_state.point_idx < MAX_POINTS)
	{
		// ���͵�ǰ������
		extern float rotation_red[2];
		extern void Servo_All_SetPWM(float, float);
		rotation_red[0] = shared_x_points[shape_state.point_idx];
		rotation_red[1] = shared_y_points[shape_state.point_idx];
		Servo_All_SetPWM(rotation_red[0], rotation_red[1]);

		// �������л�����һ����
		static uint8_t delay_cnt = 0;
		delay_cnt++;
		if (delay_cnt >= shape_state.period)
		{
			delay_cnt = 0;
			shape_state.point_idx++;

			// ������ɣ��رռ���
			if (shape_state.point_idx >= shape_state.total_points)
			{
				shape_state.run_flag = 0;
				shape_state.init_flag = 0;
				Laser_P_OFF();
			}
		}
	}
	else
	{
		// �쳣���������㷶Χ
		shape_state.run_flag = 0;
		shape_state.init_flag = 0;
		Laser_P_OFF();
	}
}















/* TASK1 ��ȡǦ���߶��㡢ԭ�� ��¼�������� */
//void pencil_piont_get_1(void)
//{
//	switch(Do_count1)
//	{
//		case 0:
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count1++;
//				USART3_SendByte('1');
//			}
//		break;
//		case 1:
////			if(USART3_GetRxFlag() || KeyNum == KEY3_PRES)
//			if(flag.get_one_pencil_succeed_flag == 1 || KeyNum == KEY3_PRES)
//			{
//				flag.get_one_pencil_succeed_flag = 0;
//				
//				count.pencil_count++;
//				if(count.pencil_count < 5)
//				{
//					Do_count1 = 0;
//					Beep_ms(250);
//				}
//				else
//				{
//					Beep_ms(125);
//					Delay_ms(125);
//					Beep_ms(125);
//					Do_count1 = 0;
//					count.pencil_count = 0;
//				}
//			}
//		break;
//	}
//}

/* TASK1 ��ȡǦ���߶��㡢ԭ�� ��¼PWM */
void pencil_piont_get_2(void)
{
	if(Usart2Num == 0xA0)	//KEY3 �л�PWM������С
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count1)
	{
		case 0:	//KEY5 ��ʼ�ƶ�
//			if(KeyNum == KEY5_PRES)
			if(1)
			{
				Do_count1++;
			}
		break;
		case 1:
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[1]);
//				Servo21_SetPWM(rotation_red[1]);
//				
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//				Servo22_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//				Servo21_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
////				Servo2_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//				Servo21_SetPWM(rotation_red[0]);
//			}
			
//			if(KeyNum == KEY5_PRES)
			if(Usart2Num == 0xA1)
			{
				Beep_ms(125);
				pencil_points_PWM[count.pencil_count][0] = rotation_red[0];
				pencil_points_PWM[count.pencil_count][1] = rotation_red[1];
				
				count.pencil_count++;
				if(count.pencil_count >= 5)
				{
					Delay_ms(125);
					Beep_ms(125);
					count.pencil_count = 0;
					Do_count1 = 0;
				}
			}
		break;
	}
}

/* TASK1 ��ȡǦ���߶��㡢ԭ�� ��¼���������PWM */
void pencil_piont_get_3(void)
{
	if(KeyNum == KEY3_PRES)	//KEY3 �л�PWM������С
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count1)
	{
		case 0:	//KEY5 ����TASK1
			if(KeyNum == KEY5_PRES)
			{
				Do_count1++;
				
				count.pencil_count = 0;
			}
		break;
		case 1:
			/* ��ȡ���� */
			if(KeyNum == KEY5_PRES)	//�ƶ���λ
			{
				//KEY5 ��¼PWM
				pencil_points_PWM[count.pencil_count][0] = rotation_red[0];	
				pencil_points_PWM[count.pencil_count][1] = rotation_red[1];
				
				//KEY5 ��K210��Ϣ ׼����������
				K210_SendByte('1');	
			}
			if(USART3_GetRxFlag())	//���յ������ count.pencil_count++
			{
				count.pencil_count++;
				if(count.pencil_count < 5)
				{
					Beep_ms(250);
				}
				else
				{
					Beep_ms(125);
					Delay_ms(125);
					Beep_ms(125);
					Do_count1 = 0;	//�ص�Do_count1 = 0;��������У׼
				}
			}
//			/* �ƶ���̨������� */
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
		break;
	}
}

/* TASK2 ��λ ���� */
void red_piont_reset_1(void)
{
	switch(Do_count2)
	{
		case 0:
			if(Usart2Num == 0xA2)
			{
				Do_count2++;
				
				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[4][0], pencil_points_PWM[4][1]);
			}
		break;
		case 1:
			
		break;
	}
}
/* TASK2 ��λ �ջ� */
//void red_piont_reset_2(void)
//{
//	switch(Do_count2)
//	{
//		case 0:
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count2++;
//				
//				flag.reset_flag = 1;
//				flag.stop_flag = 0;
//				flag.pencil_line_flag = 0;
//				flag.pencil_line_succeed_flag = 0;
//				flag.black_line_flag = 0;
//				flag.black_line_succeed_flag = 0;
//				flag.reach_flag = 0;

//				K210_SendByte('2');
//				set_pid_target(&pid_x, pencil_points[4][0]);
//				set_pid_target(&pid_y, pencil_points[4][1]);
//			}

//		break;
//		case 1:
//			if(flag.reset_flag == 0)
//			{
//				Do_count2++;
//			}
//		break;
//		case 2:
//			
//		break;
//	}
//}

/* TASK3 ѲǦ���� ���� 5PWM*/
//void pencil_line_1(void)
//{
//	switch(Do_count3)
//	{
//		case 0:	//����KEY5��ȥ���Ͻ�
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.wating_pencil_line_flag = 1;

//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
//			}
//		break;
//		case 1:	//ȥ���Ͻ�
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[1][0], pencil_points_PWM[1][1]);
//			}
//		break;
//		case 2:	//ȥ���½�
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], pencil_points_PWM[2][1]);
//			}
//		break;
//		case 3:	//ȥ���½�
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[3][0], pencil_points_PWM[3][1]);
//			}
//		break;
//		case 4:	//ȥ���Ͻ�
//			if(count.wating_pencil_line_count >= WATING_PENCIL_LINE_10MS)
//			{
//				Do_count3++;
//				
//				flag.wating_pencil_line_flag = 0;
//				count.wating_pencil_line_count = 0;
//				
//				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
//			}
//		break;
//		case 5:
//			
//		break;
//	}
//}

///* TASK3 ѲǦ���� �ջ� */
//void pencil_line_2(void)
//{
//	switch(Do_count3)
//	{
//		case 0:	//�����ֵ��
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;

//				flag.reset_flag = 0;
//				flag.stop_flag = 0;
//				flag.pencil_line_flag = 1;
//				flag.pencil_line_succeed_flag = 0;
//				flag.black_line_flag = 0;
//				flag.black_line_succeed_flag = 0;
//				flag.reach_flag = 0;
//				
//				Red_OFF();
//				Beep_OFF();
//				
//				USART3_SendByte('3');
//				
//				Path_Points_Get();
//				current_path_point = 0;  // �ӵ�һ��·���㿪ʼ
//			}
//		break;
//		case 1:
//			if(flag.pencil_line_succeed_flag == 1)
//			{
//				Do_count3++;
//			}
//		break;
//		case 2:
//			
//		break;
//	}
//}

///* TASK3 ѲǦ���� �ջ� ���� �յ㷴�� 5����*/
//void pencil_line_3(void)
//{
//	int16_t x_error;
//	int16_t y_error;
//	int16_t step_PWM = 5;
//	uint16_t piont_err_min = 2;
//	switch(Do_count3)
//	{
//		case 0:	//KEY5, ��K210����'3', ��ʼTASK3
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.pencil_line_count_increase_flag = 1;
//				count.current_pencil_count = 0;
//				
//				USART3_SendByte('3');
//			}
//		break;
//		case 1:	//��ԭ��pencil_points[4]�����Ͻ�pencil_points[0]
//			x_error = pencil_points[0][0] - pencil_points[4][0];
//			y_error = pencil_points[0][1] - pencil_points[4][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//xȡ�࣬��y
//			{

//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//yȡ�࣬��x
//			{
//				if		(x_error > piont_err_min)	rotation_red[0] -= step_PWM;	//x����������PWM�����
//				else if	(x_error < -piont_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())	//�켤������K210���õ�Ŀ���ķ�Χ��
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					Do_count3++;
//					Beep_ms(250);
//				}
//			}
//		break;
//		case 2:	//�����Ͻ�˳ʱ����һȦ
//			x_error = pencil_points[(count.current_pencil_count + 1) % 4][0] - pencil_points[count.current_pencil_count][0];
//			y_error = pencil_points[(count.current_pencil_count + 1) % 4][1] - pencil_points[count.current_pencil_count][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//xȡ�࣬��y
//			{
//				if		(y_error > piont_err_min)	rotation_red[1] += step_PWM;	//y����������PWM�����
//				else if	(y_error < -piont_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//yȡ�࣬��x
//			{
//				if		(x_error > piont_err_min)	rotation_red[0] -= step_PWM;	//x����������PWM�����
//				else if	(x_error < -piont_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					count.current_pencil_count++;
//					Beep_ms(250);
//					//0->1:0 1->2:1 2->3:2 3->0:3 ��0:�յ�K210��Ϣ�ͱ��4���������TASK3
//					if(count.current_pencil_count == 4)
//					{
//						Do_count3++;
//						
//						Delay_ms(250);
//						Beep_ms(250);
//					}
//				}
//			}
//		break;
//		case 3:
//			flag.pencil_line_count_increase_flag = 0;
//			count.current_pencil_count = 0;

//		break;
//	}
//	
//}

///* TASK3 ѲǦ���� �ջ� ���� �յ㷴�� 5PWM*/
//void pencil_line_4(void)
//{
//	int16_t x_error;
//	int16_t y_error;
//	int16_t step_PWM = 5;
//	uint16_t PWM_err_min = 2;
//	switch(Do_count3)
//	{
//		case 0:	//KEY5, ��K210����'3', ��ʼTASK3
//			if(KeyNum == KEY5_PRES)
//			{
//				Do_count3++;
//				
//				flag.pencil_line_count_increase_flag = 1;
//				count.current_pencil_count = 0;
//				
//				USART3_SendByte('3');
//			}
//		break;
//		case 1:	//��ԭ��rotation_red�����Ͻ�pencil_points[0]
//			x_error = pencil_points_PWM[0][0] - rotation_red[0];
//			y_error = pencil_points_PWM[0][1] - rotation_red[1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//xȡ�࣬��y
//			{
//				if		(y_error > PWM_err_min)		rotation_red[1] += step_PWM;	//��PWMΪ���ο�,���������
//				else if	(y_error < -PWM_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//yȡ�࣬��x
//			{
//				if		(x_error > PWM_err_min)		rotation_red[0] += step_PWM;
//				else if	(x_error < -PWM_err_min)	rotation_red[0] -= step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())	//�켤������K210���õ�Ŀ���ķ�Χ��
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					Do_count3++;
//					Beep_ms(250);
//				}
//			}
//		break;
//		case 2:	//�����Ͻ�˳ʱ����һȦ
//			x_error = pencil_points_PWM[(count.current_pencil_count + 1) % 4][0] - pencil_points_PWM[count.current_pencil_count][0];
//			y_error = pencil_points_PWM[(count.current_pencil_count + 1) % 4][1] - pencil_points_PWM[count.current_pencil_count][1];
//			if(count.pencil_line_count % abs(x_error) == 0)	//xȡ�࣬��y
//			{
//				if		(y_error > PWM_err_min)		rotation_red[1] += step_PWM;	//y����������PWM�����
//				else if	(y_error < -PWM_err_min)	rotation_red[1] -= step_PWM;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			if(count.pencil_line_count % abs(y_error) == 0)	//yȡ�࣬��x
//			{
//				if		(x_error > PWM_err_min)		rotation_red[0] -= step_PWM;	//x����������PWM�����
//				else if	(x_error < -PWM_err_min)	rotation_red[0] += step_PWM;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			if(USART3_GetRxFlag())
//			{
//				if(USART3_GetRxData() == 0xAA)
//				{
//					count.current_pencil_count++;
//					Beep_ms(250);
//					//0->1:0 1->2:1 2->3:2 3->0:3 ��0:�յ�K210��Ϣ�ͱ��4���������TASK3
//					if(count.current_pencil_count == 4)
//					{
//						Do_count3++;
//						
//						Delay_ms(250);
//						Beep_ms(250);
//					}
//				}
//			}
//		break;
//		case 3:
//			flag.pencil_line_count_increase_flag = 1;
//			count.current_pencil_count = 0;

//		break;
//	}
//}

/* TASK3 ѲǦ���� ���� 5PWM �ۼӲ���*/
void pencil_line_5(void)
{
	uint16_t step_PWM = 2;
	uint16_t wating_time = 2;

	switch(Do_count3)
	{
		case 0:	//����KEY5��ȥ���Ͻǣ�ֱ��
//			if(KeyNum == KEY5_PRES)
			if(1)
			{
				flag.wating_pencil_line_flag = 1;

				Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
			}
			if(count.wating_pencil_line_count >= 500)	//���㹻��ʱ��
			{
				Do_count3++;
				
				count.wating_pencil_line_count = 0;
				Beep_ms(250);
			}
		break;
		case 1:	//ȥ���Ͻ�	���� ֻ��xPWM
			if(count.wating_pencil_line_count >= wating_time)	//500ms����step_PWM��30��
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[1][0] - pencil_points_PWM[0][0]) / 2 - abs(pencil_points_PWM[1][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], pencil_points_PWM[1][1]);
				}
				if(abs(rotation_red[0] - pencil_points_PWM[1][0]) > step_PWM)
				{
					rotation_red[0] -= step_PWM;	//�ұ�PWMС��������-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					//����ӽ�Ŀ��� ֱ�� �պ��ֲ�ֻ��һ������PWMʱ����һ����������
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[1][0], pencil_points_PWM[1][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 2:	//ȥ���½�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[2][1] - pencil_points_PWM[1][1]) / 2 - abs(pencil_points_PWM[2][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - pencil_points_PWM[2][1]) > step_PWM)
				{
					rotation_red[1] += step_PWM;	//�±�PWM��������+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[2][0], pencil_points_PWM[2][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 3:	//ȥ���½�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[3][0] - pencil_points_PWM[2][0]) / 2 - abs(pencil_points_PWM[3][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], pencil_points_PWM[3][1]);
				}
				
				if(abs(rotation_red[0] - pencil_points_PWM[3][1]) > step_PWM)
				{
					rotation_red[0] += step_PWM;	//���PWM��������+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[3][0], pencil_points_PWM[3][1]);
					Do_count3++;
//					Beep_ms(250);
				}
			}
		break;
		case 4:	//ȥ���Ͻ�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(pencil_points_PWM[0][1] - pencil_points_PWM[3][1]) / 2 - abs(pencil_points_PWM[0][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - pencil_points_PWM[0][1]) > step_PWM)
				{
					rotation_red[1] -= step_PWM;	//�ϱ�PWMС��������-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(pencil_points_PWM[0][0], pencil_points_PWM[0][1]);
					
					Do_count3 = 0;
					
//					Beep_ms(250);
					flag.wating_pencil_line_flag = 0;
					count.wating_pencil_line_count = 0;
				}
			}
		break;
	}
}

/* TASK4 ��ȡ�ڿ�ǵ� ��¼PWM */
void black_piont_get_guding_1(void)
{
	if(KeyNum == KEY3_PRES)	//KEY3 �л�PWM������С
	{
		PWM_step_for_TASK1 = (PWM_step_for_TASK1 == PWM_STEP_LARGE ? PWM_STEP_SMALL : PWM_STEP_LARGE);
	}
	switch(Do_count4)
	{
		case 0:	//KEY5 ��ʼ�ƶ�
			if(KeyNum == KEY5_PRES)
			{
				Do_count4++;
			}
		break;
		case 1:
//			if(KeyNum == KEY_UP_PRES)
//			{
////				PWM2_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] -= PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_DOWN_PRES)
//			{
////				PWM2_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[1] += PWM_step_for_TASK1;
//				Servo2_SetPWM(rotation_red[1]);
//			}
//			else if(KeyNum == KEY_LEFT_PRES)
//			{
////				PWM1_for_TASK1 += PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] += PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
//			else if(KeyNum == KEY_RIGHT_PRES)
//			{
////				PWM1_for_TASK1 -= PWM_step_for_TASK1;
////				Servo_All_SetPWM_ChangeRotationRed(PWM1_for_TASK1, PWM2_for_TASK1);
//				rotation_red[0] -= PWM_step_for_TASK1;
//				Servo1_SetPWM(rotation_red[0]);
//			}
			
			if(KeyNum == KEY5_PRES)
			{
				Beep_ms(125);
				black_points_PWM_guding[count.black_count_3][0] = rotation_red[0];
				black_points_PWM_guding[count.black_count_3][1] = rotation_red[1];
				
				count.black_count_3++;
				if(count.black_count_3 >= 4)	//�������ĸ��� ����ʱ4 ����֮ǰ��5
				{
					Delay_ms(125);
					Beep_ms(125);
					count.black_count_3 = 0;
					Do_count4 = 0;
				}
			}
		break;
	}
}

/* TASK5 Ѳ�ڿ� */
void black_line_guding_1(void)
{
	uint16_t step_PWM = 2;
	uint16_t wating_time = 5;

	switch(Do_count5)
	{
		case 0:	//����KEY5��ȥ���Ͻǣ�ֱ��
			if(KeyNum == KEY5_PRES)
			{
				flag.wating_pencil_line_flag = 1;

				Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], black_points_PWM_guding[0][1]);
			}
			if(count.wating_pencil_line_count >= 500)	//���㹻��ʱ��
			{
				Do_count5++;
				
				count.wating_pencil_line_count = 0;
//				Beep_ms(250);
			}
		break;
		case 1:	//ȥ���Ͻ�	���� ֻ��xPWM
			if(count.wating_pencil_line_count >= wating_time)	//500ms����step_PWM��30��
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[1][0] - black_points_PWM_guding[0][0]) / 2 - abs(black_points_PWM_guding[1][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], black_points_PWM_guding[1][1]);
				}
				if(abs(rotation_red[0] - black_points_PWM_guding[1][0]) > step_PWM)
				{
					rotation_red[0] -= step_PWM;	//�ұ�PWMС��������-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					//����ӽ�Ŀ��� ֱ�� �պ��ֲ�ֻ��һ������PWMʱ����һ����������
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[1][0], black_points_PWM_guding[1][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 2:	//ȥ���½�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[2][1] - black_points_PWM_guding[1][1]) / 2 - abs(black_points_PWM_guding[2][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[2][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - black_points_PWM_guding[2][1]) > step_PWM)
				{
					rotation_red[1] += step_PWM;	//�±�PWM��������+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[2][0], black_points_PWM_guding[2][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 3:	//ȥ���½�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[3][0] - black_points_PWM_guding[2][0]) / 2 - abs(black_points_PWM_guding[3][0] - rotation_red[0])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(rotation_red[0], black_points_PWM_guding[3][1]);
				}
				
				if(abs(rotation_red[0] - black_points_PWM_guding[3][1]) > step_PWM)
				{
					rotation_red[0] += step_PWM;	//���PWM��������+
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[3][0], black_points_PWM_guding[3][1]);
					Do_count5++;
//					Beep_ms(250);
				}
			}
		break;
		case 4:	//ȥ���Ͻ�	����
			if(count.wating_pencil_line_count >= wating_time)
			{
				count.wating_pencil_line_count = 0;
				
				if(abs(abs(black_points_PWM_guding[0][1] - black_points_PWM_guding[3][1]) / 2 - abs(black_points_PWM_guding[0][1] - rotation_red[1])) <=  step_PWM)
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], rotation_red[1]);
				}
				
				if(abs(rotation_red[1] - black_points_PWM_guding[0][1]) > step_PWM)
				{
					rotation_red[1] -= step_PWM;	//�ϱ�PWMС��������-
					Servo_All_SetPWM(rotation_red[0], rotation_red[1]);
				}
				else
				{
					Servo_All_SetPWM_ChangeRotationRed(black_points_PWM_guding[0][0], black_points_PWM_guding[0][1]);
					
					Do_count5 = 0;
					
//					Beep_ms(250);
					flag.wating_pencil_line_flag = 0;
					count.wating_pencil_line_count = 0;
				}
			}
		break;
	}
}

void black_piont_get_suiji_1(void)
{
	switch(Do_count6)
	{
		case 0:	//����'6'����ʼ����
			if(KeyNum == KEY5_PRES)
			{
				Do_count6++;
				flag.black_pionts_get_flag = 1;	//�ȴ�����
				K210_SendByte('6');
			}
		break;
		case 1:	//���յ�������·����
			if(flag.black_pionts_get_flag == 2)	//���յ�
			{
				Do_count6++;
				
				black_path_points_get();
				Beep_ms(250);
			}
		break;
		case 2:	//������
//			if(flag.black_path_pionts_get_succeed_flag == 1 && KeyNum == KEY5_PRES)
			if(flag.black_path_pionts_get_succeed_flag == 1)
			{
				Delay_ms(500);
				Beep_ms(250);
				
				TASK = 7; 
				Do_count7 = 0;
				flag.black_path_pionts_get_succeed_flag = 0;
			}
		break;
	}
}

/* TASK7 Ѳ�ڿ� �ջ� */
void black_line_1(void)
{
	switch(Do_count7)
	{
		case 0:	//���͡�7������ʼѲ��
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count7++;
				
				flag.counting_flag = 1;	//��ʼ��ʱ
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 1;	//�߾��ο�
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('7');
				black_current_path_point = 0;  // �ӵ�һ��·���㿪ʼ
			}
		break;
		case 1:
			if(flag.black_line_succeed_flag == 1)
			{
				flag.counting_flag =0;	//��ʱ����
				flag.black_line_flag = 0;
				Do_count7 = 0;
			}
		break;
		case 2:
			
		break;
	}
}
/* TASK8 ��λ׷��  */
void reset_zhuizong_1(void)
{
	switch(Do_count8)
	{
		case 0:	//���͡�8������ʼ׷��
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count8++;
				
				flag.counting_flag = 1;	//��ʼ��ʱ
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 0;	
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 0;	
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 1;	//��λ׷��
				flag.green_reset_succeed_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('8');
			}
		break;
		case 1:
			if(flag.green_reset_succeed_flag == 1)
			{
				flag.counting_flag =0;	//��ʱ����
				flag.green_reset_flag = 0;
				Do_count8 = 0;
//				K210_SendByte('!');
			}
		break;
		case 2:
			
		break;
	}
}
/* TASK9 �˶�׷��  */
void yundong_zhuizong_1(void)
{
	switch(Do_count9)
	{
		case 0:	//���͡�9������ʼ׷��
//			if(1)
			if(KeyNum == KEY5_PRES)
			{
				Do_count9++;
				
				flag.counting_flag = 1;	//��ʼ��ʱ
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 1;	//�߾��ο�
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 1;	//׷��
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_succeed_flag = 0;
				
				Red_OFF();
				Beep_OFF();
				
				K210_SendByte('9');
			}
		break;
		case 1:
			//��� �켤������ �� �̼���׷��
			if(flag.black_line_succeed_flag && flag.zhuizong_succeed_flag)
			{
				flag.counting_flag =0;	//��ʱ����
				
				flag.black_line_flag = 0;
				flag.zhuizong_flag = 0;
				Do_count9 = 0;
				K210_SendByte('!');
			}
		break;
		case 2:
			
		break;
	}
}

/* TASK10 Ŀ���ʵ�ʵ�  */
void tar_cur_pionts(void)
{
	switch(Do_count10)
	{
		case 0:	//���͡�A������ʼ׷��
//			if(1)
			if(KeyNum == KEY3_PRES)
			{
				Do_count10++;
				
				flag.counting_flag = 1;	//��ʼ��ʱ
				count.counting_time = 0;

				flag.reset_flag = 0;
				flag.stop_flag = 0;
				flag.pencil_line_flag = 0;
				flag.pencil_line_succeed_flag = 0;
				flag.black_line_flag = 0;	//�߾��ο�
				flag.black_line_succeed_flag = 0;
				flag.reach_flag = 0;
				flag.zhuizong_flag = 0;	//׷��
				flag.zhuizong_succeed_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_flag = 0;
				flag.green_reset_succeed_flag = 0;
				
				flag.tar_cur_flag = 1;
				flag.tar_cur_succeed_flag = 0;
				
				Beep_ms_TIM(500);
//				Red_OFF();
//				Beep_OFF();
				
				K210_SendByte(0x0A);
			}
		break;
		case 1:
			//��� �켤������ �� �̼���׷��
			if(flag.cur_catch_tar_flag == 1)
			{
				flag.cur_catch_tar_flag = 0;
				flag.counting_flag =0;	//��ʱ����
				
				flag.tar_cur_flag = 0;
				flag.tar_cur_succeed_flag = 1;
				Do_count10 = 0;
				
				Beep_ms_TIM(1000);
				K210_SendByte('Y');
			}
		break;
		case 2:
			
		break;
	}
}
void open_line_task(void)
{
	
}
