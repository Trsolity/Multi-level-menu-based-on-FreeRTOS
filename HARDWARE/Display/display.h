#ifndef _DISPLAY_H
#define _DISPLAY_H	 

#include "MPU6050.h"
#include "oled.h"

/*�궨����*/


/*�ⲿ����������*/


/*����������*/

void Display_Menu(u8 x);//��ʾ�˵�

void Display_Cursor(u8 x,u8 y,u8 z,u8 i);//��ʾ���(���ο�

void Display_Down_Cursor(u8 x,u8 i);//��ʾ�ײ��Ĺ��(���ο�

void Display_Time(void);//��ʾʱ���ҳ��

void Display_MPU6050(void);//��ʾmpu6050����

void Display_Choice(u8 c);

#endif

